"""
Python module to create custom magics for the IDL IPython (Jupyter)
notebook kernel. Within this module, you can create class definitions
to add custom commands as the first line in a notebook cell.

You can use this to create custom magics which enable other languages
within the IDL kernel. This will create a bridge between IDL and any
other language that also has access to Python. 

######
NOTE: If you don't have the necessary permissions to modify this file in
the default location, you can make a copy and place it on Python's search path
so that it will be found before the original file.
######

When creating custom magics, there are two steps that you need to take.

First, define a function to run for the custom magic.
Each custom magic function must have 5 arguments:

    idlkernelref (input): a reference to the IDL_kernel object. This allows you
        to invoke any methods for that class such as idl_kernel.idl_do_execute.

    codelines (input): a list containing the lines from the current
        notebook cell.

    call_result (output): a list containing the text or html that you want to
        send back to the notebook. You should use call_result.append(yourtext)
        to make sure you don't overwrite any pending output.

    graphics (output): a dictionary for returning IDL plot windows.

    globals (input): This represents the globals() variable scope in the IDL
        kernel definition, and could be useful for certain magic functions.

Second, add the magic identifier to the magics dictionary like:
    IDL_Magic['custom_string'] = my_method_to_run

See below for examples.
"""


# Import modules for custom magics.
#for python_do_execute
import sys
import traceback
from idlpy import *

IDL_Magic = {}


class IDL_magic_stdout():
    """
    Override the Python stdout.
    >>> prevStdout = sys.stdout
    >>> idlstdout = IDL_magic_stdout()
    >>> sys.stdout = idlstdout
    >>> 2+2
    >>> sys.stdout = prevStdout
    >>> print(idlstdout.data)
    4
    <BLANKLINE>
    """
    def __init__(self):
        """>>> pass          # IDL_magic_stdout.__init__()"""
        self.data = ''
        self.orig_stdout = sys.stdout
    def write(self, s):
        """>>> pass          # IDL_magic_stdout.write()"""
        self.data += s
    def flush(self):
        """>>> pass          # IDL_magic_stdout.flush()"""
        pass


def idl_magic_lsmagic(idlkernelref, codelines, call_result, graphics, globals):
    """
    List currently available magic functions.

        %lsmagic

    """

    keys = list(IDL_Magic.keys())
    keys.sort()
    call_result.append("\n".join(keys))

IDL_Magic['%lsmagic'] = idl_magic_lsmagic



def idl_magic_html(idlkernelref, codelines, call_result, graphics, globals):
    """
    Allow HTML to be output in the cell.

        %%html
        <ul><li>IDL<li>is<li>fun!</ul>

    """
    codelines[0] = ''
    call_result.append("<HTML>" + "".join(codelines) + "</HTML>")

IDL_Magic['%%html'] = idl_magic_html



def idl_magic_python_do_execute(idlkernelref, codelines, call_result, graphics, globals):
    """
    Magic to execute Python code.

        >>>
        temp = 5
        def function1()
            print('Called function 1!')
            return (1)
        print(temp)
        void = function1()

    The result would then be:
        
        5
        Called function 1!

    Because we use globals in the exec statement, we can also access
    these new Python functions from IDL:
        idltemp = Python.temp
        void = Python.function1()
    """
    # Replace first line so that python can compile and run the code
    codelines[0] = ''
    py_code = '\n'.join(codelines)

    # Redirect stdout so we can return it to the notebook
    stdout = IDL_magic_stdout()
    orig_stdout = sys.stdout
    sys.stdout = stdout

    try:
        # Run exec in global scope
        exec(py_code, globals)
        # Get any pending results
        data = stdout.data
        if data != '':
            call_result.append(data.rstrip())
    except:
        # Get any pending results
        data = stdout.data
        if data != '':
            call_result.append(data.rstrip())
        # Return the error that was raised
        err = str(traceback.format_exc())
        result = err.replace('\n', '<br>')
        result = "<html><pre><font color='red'>" + result + "</font></pre></html>"
        call_result.append(result)

    sys.stdout = orig_stdout

IDL_Magic['>>>'] = idl_magic_python_do_execute
IDL_Magic['%%python'] = idl_magic_python_do_execute



def idl_magic_config(idlkernelref, codelines, call_result, graphics, globals):
    """
    Magic to change a configuration value.

        %config                     <-- get a list of configuration properties
        %config textcolor           <-- get the current property value
        %config textcolor = blue    <-- set the property value

    """
    arg = codelines[0].split("=")
    if len(arg) > 1:
        value = arg[1]
    else:
        value = None

    arg = arg[0].split()
    configs = idlkernelref.configs
    if (len(arg) <= 1):
        call_result.append('\n'.join(configs))
    elif (value == None):
        try:
            call_result.append(arg[1] + " = " + configs[arg[1]])
        except:
            call_result.append("Unknown configuration property: " + arg[1])
    else:
        configs[arg[1]] = value

IDL_Magic['%config'] = idl_magic_config



def idl_magic_inline(idlkernelref, codelines, call_result, graphics, globals):
    """
    Magic to change IDL graphics to inline (embedded) within the notebook.

        %inline

    """
    IDL.run("!MAGIC.embed=1", silent=True)

IDL_Magic['%inline'] = idl_magic_inline



def idl_magic_window(idlkernelref, codelines, call_result, graphics, globals):
    """
    Magic to change IDL graphics to separate windows.

        %window

    """
    IDL.run("!MAGIC.embed=0", silent=True)

IDL_Magic['%window'] = idl_magic_window



def idl_magic_load_code(idlkernelref, codelines, call_result, graphics, globals):
    """
    Trying to figure out how to do something like .edit in the jupyter notebook kernel
    Can't get the messaging quite right, not sure what's wrong exactly. 
    """
    payload = {
        "source": "set_next_input",
        # the text contents of the cell to create
        "text": "some cell content",
        # If true, replace the current cell in document UIs instead of inserting
        # a cell. Ignored in console UIs.
        "replace": "false"
    }

    content = {
        'status':'ok',
        'payload':[payload] #list of payload
    }

    idlkernelref.send_response(idlkernelref.stdin_socket, 'execute_reply', content)

# Don't include this since it doesn't currently work
# IDL_Magic['%load'] = idl_magic_load_code
