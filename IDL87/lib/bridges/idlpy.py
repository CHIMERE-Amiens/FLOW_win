# Copyright (c)  Harris Geospatial Solutions, Inc. All
# rights reserved. This software includes information which is
# proprietary to and a trade secret of Harris Geospatial Solutions, Inc.
# It is not to be disclosed to anyone outside of this organization.
# Reproduction by any means whatsoever is prohibited without express
# written permission.
#
# Unit Tests:
#   To run the Python doctest unit tests, do:
#   >>> python -m doctest -v idlpy.py
#

"""
This is the IDL module that implements the Python-to-IDL bridge.
>>> pass          # idlpy module
"""

import sys
import os
# import traceback

# Dynamically construct the library name from the Python major.minor version.
idllibrary = "pythonidl" + str(sys.version_info[0]) + str(sys.version_info[1])
pyidl = __import__(idllibrary)

# Add IDL's bin directory to the PATH environment variable.
# This is needed so IDL can find the licensing libraries.
idldir = os.path.dirname(pyidl.__file__)
os.environ['PATH'] = os.environ['PATH'] + os.pathsep + idldir

def exitHandler():
    if pyidl != None:
        pyidl.cleanup()

import atexit
atexit.register(exitHandler)

class IDL_stdout():
    """
    Override the Python stdout so we can check the IDL output for IDL errors.
    >>> prevStdout = sys.stdout
    >>> idlstdout = IDL_stdout(True)
    >>> sys.stdout = idlstdout
    >>> 2+2
    >>> sys.stdout = prevStdout
    >>> print(idlstdout.data)
    4
    <BLANKLINE>
    """
    def __init__(self, capture=False):
        """>>> pass          # IDL_stdout.__init__()"""
        if (capture):
            self.data = ''
        else:
            self.data = None
        self.orig_stdout = sys.stdout
        self.idlErrorOccurred = False
    def write(self, s):
        """>>> pass          # IDL_stdout.write()"""
        if not self.idlErrorOccurred:
            if "% Execution halted at: " in s:
                self.idlErrorOccurred = True
            elif "% Syntax error." in s:
                self.idlErrorOccurred = True
            elif "Programs can't be compiled" in s:
                self.idlErrorOccurred = True
        if (self.data == None):
            self.orig_stdout.write(s)
        else:
            self.data += s
    def flush(self):
        """>>> pass          # IDL_stdout.flush()"""
        if (self.data == None):
            self.orig_stdout.flush()


class IDLError(RuntimeError):
    """
    This is the exception that is thrown for IDL syntax or runtime errors.

    For example:
    >>> try:   # doctest: +ELLIPSIS
    ...     r = IDL.dist()
    ...     print("should not reach here")
    ... except IDLError as e:
    ...     print('IDLError: "' + str(e) + '"')
    % Compiled module: DIST.
    % Variable is undefined: N.
    % Execution halted at: $MAIN$...
    IDLError: "% Variable is undefined: N."
    """
    # We just subclass from RuntimeError and don't add any functionality.
    pass


def IDLStartCall( capture = False ):
    """
    Helper function to clear IDL's error state before an IDL call,
    and wire up IDL's stdout to Python's stdout.
    >>> IDLStartCall()
    >>> IDLFinishCall()
    (False, None)
    """
    pyidl.callFunction("MESSAGE", (), {"RESET":1})
    idlStdout = IDL_stdout( capture = capture )
    sys.stdout = idlStdout


def IDLFinishCall():
    """
    Helper function to retrieve any IDL output from an IDL call
    and check IDL for errors.
    Returns a tuple of "err, output" where "output" is either None or a string
    containing the IDL output, and "err" is either None or the error message.
    >>> IDLStartCall()
    >>> IDLFinishCall()
    (False, None)
    """
    output = None
    err = False
    errorState = pyidl.getVariable("!ERROR_STATE")
    if not isinstance(sys.stdout, IDL_stdout):
        raise RuntimeError("sys.stdout is not an instance of IDL_stdout")
    err = sys.stdout.idlErrorOccurred
    output = sys.stdout.data
    sys.stdout = sys.stdout.orig_stdout
    if output != None:
        output = output.strip('\n')
        if output == '':
            output = None
    if (err and output == None):
        output = errorState['MSG_PREFIX'] + errorState['MSG']
    return err, output


# subclass from type to create our own metaclass
class IDLType(type):
    """
    Metaclass for IDL
    >>> pass          # IDLType class
    """
    def __init__(cls, name, bases, attrs):
        """
        Initialize the IDL interpreter.
        >>> pass          # IDLType.__init__
        """
        super(IDLType, cls).__init__(name, bases, attrs)
        try:
            pyidl.initialize()
        except Exception as err:
            e = repr(err)     
            if all(s in e for s in ['Acquire','feature','not','found']):
                rErr = RuntimeError('Unable to acquire an IDL license.')
                rErr.__cause__ = None
                raise rErr
            else:
                raise

    @classmethod
    def __getattr__(cls, attr, extra=None):
        """
        Retrieve an IDL variable or call an IDL routine.
        
        For example, to retrieve a variable:
        >>> IDL.run("var = 'Hello, World!'")
        >>> var = IDL.var
        >>> print(var)
        Hello, World!

        Retrieve an IDL system variable:
        >>> print('%.10f' % getattr(IDL, "!DPI"))
        3.1415926536

        Another example, calling an IDL routine:
        >>> result = IDL.findgen(10,20)
        >>> result.shape   # doctest: +ELLIPSIS
        (20..., 10...)
        """

        # In Python3 we get called with an extra argument from "help(IDL)"
        if (extra != None):
            return super(cls, attr).__getattr__(extra)

        # For internal Python attributes, just call our superclass
        if (attr[0] == '_' or attr == 'getdoc'):
            return getattr(super(IDLType, cls), attr)

        #*** print("class __getattr__: " + attr)
        # we need to ask IDL if this variable exists
        # if it doesn't, then we assume that it is an IDL function call.
        # unfortunately, this won't throw an error for mispelled variables...
        if (pyidl.hasVariable(attr)):
            return pyidl.getVariable(attr)

        return IDLRoutine(attr)

    @classmethod
    def __setattr__(cls, attr, value):
        """
        Set an IDL variable value.
        
        Pass a numpy array:
        >>> import numpy as np
        >>> a = np.asarray([1.0,2,3])
        >>> IDL.a = a
        >>> IDL.run('help, a & print,a', stdout=True)
        A               DOUBLE    = Array[3]
               1.0000000       2.0000000       3.0000000

        >>> array = np.asarray(['item1', 'item2', 'item3'])
        >>> IDL.array = array
        >>> IDL.run('help, array & print, array', stdout=True)
        ARRAY           STRING    = Array[3]
        item1 item2 item3

        Pass a dictionary:
        >>> IDL.var = {'key1':5, 'key2': "value"}
        >>> IDL.run('PRINT, var, /IMPLIED')
        '{\\n    "key1": 5,\\n    "key2": "value"\\n}'

        Set an IDL system variable:
        >>> setattr(IDL, "!QUIET", 1)
        >>> getattr(IDL, "!QUIET")
        1
        """
        #*** print("class __setattr__")
        # add try/catch and fix up error msg
        pyidl.setVariable(attr, value)

        
class IDLRoutine(object):
    """
    Used to call an IDL function, procedure, or static method.
    >>> r = IDLRoutine("PRINT")
    >>> r("hello")
    hello
    """
    def __init__(self, fn):
        """>>> pass          # IDLRoutine.__init__()"""
        self.__dict__['fn'] = fn

    def __call__(self, *args, **kw):
        """
        >>> a = IDL.Lindgen
        >>> a(5)   # doctest: +ELLIPSIS
        array([0, 1, 2, 3, 4]...)

        >>> e = IDL.Execute
        >>> success = e("print,2+2")
               4
        """
        #*** print "%s%s%s" % (fn, args, kw)

        # Watch out for our special IDL routines. These are not
        # in the IDL symbol table, so they need to be handled specially.
        if (self.fn.lower() == "call_function" or self.fn.lower() == "call_procedure"):
            # Replace the function name and shove the args over to the left
            self.fn = args[0]
            args = args[1:]

        # Watch out for EXECUTE. In this case just fix up the string
        # and route it directly to pyidl.execute.
        # This does not handle the optional EXECUTE arguments.
        if (self.fn.lower() == "execute"):
            squote = args[0].find("'")
            dquote = args[0].find("\"")
            if squote >= 0 and (dquote == -1 or squote < dquote):
                quote = "\""
            else:
                quote = "'"
            IDLStartCall()
            # Use our hopefully un-used !c to store the result.
            pyidl.execute("!c = execute(" + quote + args[0] + quote + ")")
            err, output = IDLFinishCall()
            if err: raise IDLError(output)
            return pyidl.getVariable("!c")

        # To be able to call IDL procedures as functions, we need to
        # make sure that the IDL routine has been compiled.
        # Use /QUIET to ignore errors from non-existent or system routines.
        pyidl.callFunction("Resolve_Routine", (self.fn,),
            {"QUIET":1, "EITHER":1, "NO_RECOMPILE":0})

        IDLStartCall()
        # callFunction is a Python method in our C library.
        result = pyidl.callFunction(self.fn, args, kw)
        err, output = IDLFinishCall()
        if err: raise IDLError(output)
        return result


    # If the user does IDL.MyThing.MySubthing, then assume that this is
    # a static method call. Example: IDL.Clipboard.Get()
    def __getattr__(self, attr):
        """
        Do an IDL static method call.
        
        For example:
        >>> IDL.Clipboard.Set("hello")
        >>> IDL.Clipboard.Get()
        ['hello']
        """
        # For internal Python attributes, just call our superclass
        if (attr[0] == '_' or attr == 'getdoc'):
            return getattr(super(IDLRoutine, self), attr)
        # A generic wrapper that can call an IDL static method.
        def wrapper(*args, **kw):
            #*** print "%s%s%s" % (attr, args, kw)
            # Build up the name of the static method.
            staticMethod = self.fn + "." + attr
            # Add a fake HVID
            args = list(args)
            args.insert(0, -1)
            args = tuple(args)
            IDLStartCall()
            result = pyidl.callMethod(staticMethod, args, kw)
            err, output = IDLFinishCall()
            if err: raise IDLError(output)
            return result
        return wrapper

            
# Use a metaclass so we can have class methods (such as "run") and attributes
# that can only be invoked on the class, not an instance.
class IDL(IDLType("IDL", (object,), {})):
    """
    Used to invoke the run method, and get or set IDL $main$ variables.
    Also serves as the Python class for wrapping IDL objects.
    >>> pass          # IDL class
    """
    
    # hvid is the IDL object heap identifier
    def __init__(self, hvid):
        """
        Create a Python object that wraps an IDL object with heap ID HVID.
        >>> pass          # IDL.__init__()
        """
        if (hvid <= 0):
            raise ValueError("A valid IDL HVID must be passed in.")
        # traceback.print_stack()
        self.hvid = hvid
        # On construction, increment the IDL reference count
        pyidl.heapIncrRefCount(self.hvid)
        # Replace our class methods with ones that throws an error.
        def run(command, stdout=False):
            raise ValueError("IDL.run() must be called as a class method.")
        self.__dict__['run'] = run
        def exit():
            raise ValueError("IDL.exit() must be called as a class method.")
        self.__dict__['exit'] = exit

    def __del__(self):
        """
        On destruction of an IDL object, decrement the IDL reference count.
        >>> pass          # IDL.__del__()
        """
        # Use a try/except in case IDL has already exited.
        try:
            pyidl.heapDecrRefCount(self.hvid)
        except Exception:
            pass


    def __repr__(self):
        """
        Create a representation of the IDL object using implied print.
        >>> obj = IDL.Obj_New("IDLgrModel")
        >>> repr(obj)   # doctest: +ELLIPSIS
        '<ObjHeapVar...(IDLGRMODEL)>'
        """
        strlist = IDL.string(self, implied=1)
        # If the result is a scalar string, just return it.
        if (strlist.__class__ == str): return strlist
        # Otherwise, if we have a list of strings, concat them together
        result = ''
        for idx, item in enumerate(strlist):
            result += item + '\n'
        return result

        
    def __str__(self):
        """
        Return a brief description of the IDL object.
        >>> obj = IDL.Obj_New("IDLgrModel")
        >>> print(obj)   # doctest: +ELLIPSIS
        idlpy.IDL <...>  IDL Class: IDLGRMODEL <HVID=...>
        """
        idlClass = pyidl.callFunction("Obj_Class", (self,))
        format = "%s.%s <%i>  IDL Class: %s <HVID=%i>"
        return format % (self.__module__, self.__class__.__name__,
            id(self), idlClass, self.hvid)


    # Override the magic method for getattr for an IDL object
    # Note: This will not override getattr for the class
    def __getattr__(self, attr):
        """
        Retrieve a property value or call a method on the IDL object.
        
        For example, retrieve a property:
        >>> p = IDL.plot(test=1)    # doctest: +SKIP
        >>> p.color    # doctest: +SKIP
        array([0, 0, 0], dtype=uint8)

        Another example, call a method:
        >>> result = p.copywindow()    # doctest: +SKIP
        >>> result.shape   # doctest: +SKIP
        (512..., 640..., 3...)
        >>> p.close()    # doctest: +SKIP
        """
        # For internal Python attributes, just call our superclass
        if (attr[0] == '_'):
            return getattr(super(IDL, self), attr)

        #*** print("instance __getattr__: " + attr)

        isMethod = pyidl.callFunction("Obj_HasMethod", (self, attr,))

        if (isMethod):
            # A generic wrapper that can call an IDL method.
            def wrapper(*args, **kw):
                #*** print "%s%s%s" % (attr, args, kw)
                args = list(args)
                args.insert(0, self.hvid)
                args = tuple(args)
                IDLStartCall()
                result = pyidl.callMethod(attr, args, kw)
                err, output = IDLFinishCall()
                if err: raise IDLError(output)
                return result
            return wrapper
        else:
            # otherwise retrieve the property from the IDL object
            args = (self.hvid, attr)
            IDLStartCall()
            result = pyidl.callMethod("GetPropByName", args)
            err, output = IDLFinishCall()
            if err: raise IDLError(output)
            return result

            
    def __setattr__(self, attr, value):
        """
        Set a property value on the IDL object.
        
        For example:
        >>> p = IDL.plot(test=1)    # doctest: +SKIP
        >>> p.color = 'red'    # doctest: +SKIP
        >>> p.color    # doctest: +SKIP
        array([255,   0,   0], dtype=uint8)
        """
        if (attr == "hvid"):
            self.__dict__['hvid'] = value
            return
        #*** print("instance __setattr__: " + attr)
        args = (self.hvid,)
        kwArgs = {}
        kwArgs.setdefault(attr, value)
        IDLStartCall()
        pyidl.callMethod("SetProperty", args, kwArgs)
        err, output = IDLFinishCall()
        if err: raise IDLError(output)

        
    # We could have defined this in the IDLType above.
    # By defining it here the docs will show up with help(IDL).
    @classmethod
    def run(cls, command, stdout=False, silent=False):
        """
        Execute an IDL command.
        
        For example:
        >>> IDL.run('var = LINDGEN(100)')
        >>> IDL.run('print, TOTAL(var)')
        '      4950.00'

        Syntax or runtime errors will throw an IDLError exception:
        >>> try:
        ...     IDL.run('var = LINDGEN(', stdout=True)
        ...     print("should not reach here")
        ... except IDLError as e:
        ...     print('IDLError: "' + str(e) + '"')
        <BLANKLINE>
        var = LINDGEN(
                      ^
        % Syntax error.
        IDLError: "
        % Syntax error."
        """
        try:
            IDLStartCall(capture = not stdout)
            pyidl.execute(command, silent=silent)
        except Exception:
            pass

        # Retrieve output and check for any IDL errors.
        # Note that for stdout=False we use the pending IDL output
        # for the error message. This should include the error itself.
        err, output = IDLFinishCall()
        if err:
            if not output: output = ''
            raise IDLError('\n' + output)

        return output


    # We could have defined this in the IDLType above.
    # By defining it here the docs will show up with help(IDL).
    @classmethod
    def exit(cls):
        """
        Exit the IDL process.
        >>> IDL.exit()    # doctest: +SKIP
        >>> pass          # IDL.exit
        """
        pyidl.cleanup()
