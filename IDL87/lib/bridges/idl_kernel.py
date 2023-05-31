# IDL IPython (Jupyter) Kernel
#
# Copyright (c)  Harris Geospatial Solutions, Inc. All
# rights reserved. This software includes information which is
# proprietary to and a trade secret of Harris Geospatial Solutions, Inc.
# It is not to be disclosed to anyone outside of this organization.
# Reproduction by any means whatsoever is prohibited without express
# written permission.
#
# See the README file for details on installation and running.
#
# Unit Tests:
#   To run the Python doctest unit tests, do:
#   >>> python -m doctest -v idl_kernel.py
#
from ipykernel.kernelbase import Kernel
from idlpy import *
from idl_magic import IDL_Magic

import collections
import re
import os
import tempfile
import hashlib

__version__ = '1.0.3'

# -----------------------------------------------------------------------------


class IDL_kernel(Kernel):
    """
    IDL IPython (Jupyter) Kernel class.
    >>> idl = IDL_kernel()
    """
    implementation = 'idl_kernel'
    implementation_version = __version__
    language = 'IDL'
    language_info = {
        'name': 'idl',
        'codemirror_mode': 'idl',
        'mimetype': 'text/x-idl',
        'file_extension': '.pro'
    }
    _banner = "IDL"
    _graphicid = '__IDLGRAPHICPLACEHOLDER__'
    codeComplete = None
    history = None
    # This is the list of configuration properties that can be set using
    # the %config magic.
    configs = {"textcolor": "", "errorcolor": "red"}


    # -------------------------------------------------------------------------
    @property
    def banner(self):
        """
        >>> idl = IDL_kernel()
        >>> idl.banner
        'IDL'
        """
        return self._banner


    # -------------------------------------------------------------------------
    def __init__(self, **kwargs):
        """
        >>> idl = IDL_kernel()
        """
        Kernel.__init__(self, **kwargs)
        self.idl_post_reset()


    # -------------------------------------------------------------------------
    def idl_post_reset(self):
        """
        Reset the IDL Kernel after a .RESET or .FULL_RESET.
        >>> idl = IDL_kernel()
        >>> idl.idl_post_reset()
        >>> magic = getattr(IDL, "!MAGIC")
        >>> magic['EMBED']
        1
        """
        IDL.run("compile_opt idl2", silent=True)
        IDL.run("!QUIET=1 & !MAGIC.embed=1 & !MAGIC.window=-1", silent=True)


    # -------------------------------------------------------------------------
    def idl_post_execute(self):
        """
        Reset the IDL !MAGIC system variable.
        >>> idl = IDL_kernel()
        >>> idl.idl_post_execute()
        >>> magic = getattr(IDL, "!MAGIC")
        >>> magic['WINDOW']
        -1
        """
        IDL.run("!MAGIC.window=-1", silent=True)


    # -------------------------------------------------------------------------
    def idl_make_error(self, e):
        """
        Construct an HTML error message from an Exception e.
        >>> IDL_kernel.idl_make_error(RuntimeError("MyError")) # doctest: +ELLIPSIS
        "<html>...MyError...</html>"
        """

        result = str(e)
        # Don't need the leading newline
        if result[0] == '\n':
            result = result[1:]
        # Replace any newlines with html breaks
        result = result.replace('\n', '<br>')
        result = "<html><pre><font color='" + self.configs["errorcolor"] + \
            "'>" + result + "</font></pre></html>"
        return result


    # -------------------------------------------------------------------------
    def idl_output_result(self, result, graphics):
        """
        Process IDL output and route to a PNG, HTML, or plain text.
        >>> pass
        """

        # Check if PNG
        if result == self._graphicid and len(graphics) > 0:
            # Returns a tuple with the window ID and !magic info.
            idlmagic = graphics.popitem(False)[1]
            img = IDL.EncodeGraphic(idlmagic['WINDOW'], idlmagic['TYPE'])
            if img != None:
                # See Python logging module
#                self.log.warning("IDL graphic " + str(idlmagic['WINDOW']) + " md5=" +
#                hashlib.md5(img.encode()).hexdigest())
                width = str(idlmagic['XSIZE'])
                height = str(idlmagic['YSIZE'])
                response = {
                    'data': { 'image/png': img },
                    'metadata': { 'image/png': {'width': width, 'height': height} }
                }
            else:
                err = 'Error: Unable to encode graphics window ' + str(idlmagic['WINDOW']) + '\n'
                response = {
                    'data': { 'text/plain': err },
                    'metadata': { }
                }
        else:
            if re.match("<html>", result, re.I):
                response = {
                    'data': { 'text/html': result + '\n' },
                    'metadata': { }
                }
            else:
                if self.configs["textcolor"] != "":
                    result = "<font color='" + self.configs["textcolor"] + "'>" + result + "</font>"
                response = {
                    'data': { 'text/html': '<html><pre>' + result + '</pre></html>' },
                    'metadata': { }
                }

        self.send_response(self.iopub_socket, 'display_data', response)


    # -------------------------------------------------------------------------
    def idl_do_execute_command(self, command, call_result, graphics):
        """
        Execute a single IDL command, store the output
        in call_result and graphics.

        For example, to execute a simple command:
        >>> idl = IDL_kernel()
        >>> call_result = []
        >>> idl.idl_do_execute_command("print,2+2", call_result, None)
        >>> call_result
        ['           4']

        Execute a graphics command:
        >>> graphics = collections.OrderedDict()
        >>> idl.idl_do_execute_command("PLOT,[0,1]", [], graphics)
        >>> graphics[0]['EMBED'], graphics[0]['WINDOW'], graphics[0]['TYPE']
        (1, 0, 0)

        IDL errors will be output:
        >>> idl = IDL_kernel()
        >>> call_result = []
        >>> idl.idl_do_execute_command("print,2+", call_result, None)
        >>> call_result   # doctest: +ELLIPSIS
        ["...% Syntax error..."]
        """
        try:
            result = IDL.run(command)
        except IDLError as e:
            call_result.append(self.idl_make_error(e))
            return

        idlmagic = getattr(IDL, '!MAGIC')
        if idlmagic['WINDOW'] != -1 and idlmagic['WINDOW'] not in graphics:
            graphics[idlmagic['WINDOW']] = idlmagic
            call_result.append(self._graphicid)
        if result is not None:
            call_result.append(result)

        if command.strip().startswith('.res') or command.strip().startswith('.f'):
            self.idl_post_reset()


    # -------------------------------------------------------------------------
    def idl_do_execute(self, code, call_result, graphics):
        """
        Execute a block of IDL commands, store the output
        in call_result and graphics.

        For example:
        >>> idl = IDL_kernel()
        >>> call_result = []
        >>> idl.idl_do_execute("print,2+2", call_result, None)
        >>> call_result
        ['           4']

        You can use multi-line statements:
        >>> call_result = []
        >>> cmd = "for i=0,2 do begin\\nprint,i\\nendfor\\nend"
        >>> idl.idl_do_execute(cmd, call_result, None)
        >>> call_result
        ['           0\\n           1\\n           2']

        You can define functions/procedures and call them:
        >>> call_result = []
        >>> cmd = "pro mytestpro\\nprint,'mytestpro'\\nend\\nmytestpro\\nend"
        >>> idl.idl_do_execute(cmd, call_result, None)
        >>> call_result
        ['mytestpro']
        """

        codelines = code.rstrip().splitlines()
        # Get the first word in the first line in case there are magics
        codebegin = codelines[0].strip().split()[0].strip()
        # Get the lastline
        codeend = codelines[-1].strip().lower()

        if codebegin.lower() in IDL_Magic:
            methodToCall = IDL_Magic[codebegin.lower()]
            methodToCall(self, codelines, call_result, graphics, globals())
        elif codebegin.startswith('%'):
            err = self.idl_make_error("Unknown ??? magic command: " + codebegin)
            call_result.append(err)
        elif codeend == 'end':
            try:
                # Write code to disk, then run it.
                f = tempfile.NamedTemporaryFile(mode='w', delete=False)
                filename = f.name
                f.write(code + "\n")
                f.close()
                command = ".run '" + filename + "'"
                self.idl_do_execute_command(command, call_result, graphics)
                os.remove(filename)
            except Exception as e:
                call_result.append(self.idl_make_error(e))
        else:
            call_line = ''
            for each in codelines:
                each = each.rstrip()
                eachstrip = each.strip().lower()

                if eachstrip == '.r' or eachstrip == '.ru' or eachstrip == '.run':
                    continue

                # If we see an IDL continutation character, just build up the
                # command over multiple lines.
                if len(each) > 0 and each[-1] == '$':
                    call_line += each[0:-1]
                    continue
                else:
                    call_line += each

                self.idl_do_execute_command(call_line, call_result, graphics)
                call_line = ''


    # -------------------------------------------------------------------------
    def do_execute(self, code, silent, store_history=True,
                   user_expressions=None, allow_stdin=False):
        """
        Execute a cell from the IDL Kernel.
        >>> idl = IDL_kernel()

        For example:
        >>> idl = IDL_kernel()
        >>> reply = idl.do_execute("print,2+2", True)
        >>> reply['status']
        'ok'
        """

        reply = {}
        call_result = []
        graphics = collections.OrderedDict()

        try:
            self.idl_do_execute(code, call_result, graphics)

        except Exception as e:
            status = u'error'
            reply['ename'] = 'Unknown error'
            reply['evalue'] = 'Unknown error'
            self.idl_output_result(self.idl_make_error(e), None)

        else:
            status = u'ok'
            if not silent:
                for line in call_result:
                    self.idl_output_result(line, graphics)


        self.idl_post_execute()

        reply[u'status'] = status
        reply['execution_count'] = self.execution_count - 1
        reply['payload'] = []

        return reply


    # -------------------------------------------------------------------------
    def processIDLCatalog(self, catFile):
        """
        Read and process the IDL tab-completion catalog.
        >>> pass  # tested by do_complete
        """
        result = []
        # Python 3 requires the encoding keyword, but Python 2 doesn't support it,
        # so use a try/except to handle both cases.
        try:
            myfile = open(catFile, encoding='utf-8')
        except Exception:
            myfile = open(catFile)
        with myfile as catalogFile:
            while True:
                line = catalogFile.readline()
                if not line: break
                pos = line.find("<ROUTINE name=")
                if (pos >= 0):
                    routine = (line.split('"'))[1]
                    if ((len(routine) > 0) and (routine.upper() == routine)):
                        result.append(routine)
        result.sort()
        return result


    # -------------------------------------------------------------------------
    # Tab completion - reads in the IDL documentation catalog file,
    # parses out the routine list and caches it.
    # Then, when the user hits tab, it searches for all matches.
    #
    # This appears to be used by ipython qtconsole, console, and notebook.
    #
    def do_complete(self, code, cursor_pos):
        """
        Tab completion for the IDL Kernel.

        For example:
        >>> idl = IDL_kernel()
        >>> result = idl.do_complete("FFT", 3)
        >>> result['matches']
        ['FFT', 'FFT_POWERSPECTRUM']
        """
        # See if we need to construct our cache
        if (IDL_kernel.codeComplete == None):
            idldir = getattr(IDL, '!DIR')
            idlCatalog = idldir + '/help/idl_catalog.xml'
            IDL_kernel.codeComplete = self.processIDLCatalog(idlCatalog)
        word = str(code[0:cursor_pos]).upper()
        # Naive regular expression for the last valid IDL identifier.
        word = (re.split('[ =+\-\*\/\(,]*', word))[-1]
        result = []
        cursor_start = cursor_pos
        if (len(word) > 0):
            for idlword in IDL_kernel.codeComplete:
                if idlword.startswith(word):
                    result.append(idlword)
            # Start the autocomplete at the beginning of the IDL identifier.
            if (len(result) > 0): cursor_start = cursor_pos - len(word)
        return {'status': 'ok', 'matches': result,
            'cursor_start': cursor_start, 'cursor_end': cursor_pos}


    # -------------------------------------------------------------------------
    # Is the command ready to execute, or does it need to continue
    # on the next line?
    #
    # This doesn't appear to be working with any of the ipython front ends.
    #
    def do_is_complete(self, code):
        """
        Determine if the command is ready to execute.
        >>> idl = IDL_kernel()
        >>> idl.do_is_complete("mycode")['status']
        'complete'
        >>> idl.do_is_complete("mycode $")['status']
        'incomplete'
        """
        status = {'status': 'complete'}
        if (code.endswith("$")):
            status = {'status': 'incomplete', 'indent': '  '}
        return status


    # -------------------------------------------------------------------------
    # This works with the qtconsole when you type a parentheses. For example:
    #    a = FFT(
    #
    def do_inspect(self, code, cursor_pos, detail_level=0):
        """
        Code introspection.
        >>> idl = IDL_kernel()
        >>> idl.do_inspect("PRINT", 5)['status']
        'ok'
        """
        reply_content = {'status' : 'ok'}
        # This should contain the "docs" for the function call.
        result = {'text/plain': 'Not yet implemented...'}
        reply_content['data'] = result
        reply_content['metadata'] = {}
        # Change this to True when we implement this.
        reply_content['found'] = False
        return reply_content


    # -------------------------------------------------------------------------
    def processIDLHistory(self, histFile):
        """
        Read and process the IDL history file.
        >>> pass  # tested by do_history
        """
        result = []
        # Python 3 requires the encoding keyword, but Python 2 doesn't support it,
        # so use a try/except to handle both cases.
        try:
            myfile = open(histFile, encoding='utf-8')
        except Exception:
            myfile = open(histFile)
        with myfile as historyFile:
            index = 0
            commands = []
            while True:
                line = historyFile.readline()
                if not line: break
                line = line.rsplit(" <!--")
                if (len(line) >= 2):
                    result.append(line[0])
                    index = index + 1
            result.reverse()
            result = [(None, None, h) for h in result]
        return result


    # -------------------------------------------------------------------------
    # This is used by the ipython qtconsole, but does not appear to be used
    # for the ipython console or ipython notebook.
    #
    def do_history(self, hist_access_type, output, raw, session=None, start=None,
        stop=None, n=None, pattern=None, unique=False):
        """
        Command history for the IDL Kernel.

        For example:
        >>> idl = IDL_kernel()
        >>> result = idl.do_history("tail", False, False, n=1)
        >>> type(result['history'][0][2])
        <class 'str'>
        """
        # See if we need to construct our cache
        if (IDL_kernel.history == None):
            from os.path import expanduser
            idldir = getattr(IDL, '!DIR')
            idlHistory = expanduser("~") + '/.idl/idl/rbuf/history'
            IDL_kernel.history = self.processIDLHistory(idlHistory)
        if (hist_access_type == "tail"):
            history = self.history[max(0,len(self.history) - n):]
        return {'history': history }


    # -------------------------------------------------------------------------
    def do_interrupt(self, interrupt):
        """
        Interrupt the IDL Kernel.
        >>> idl = IDL_kernel()
        >>> idl.do_interrupt(False)   # doctest: +ELLIPSIS
        {...}
        """
        try:
            self.log.debug("Interrupt IDL kernel...")
        except:
            pass
        return {'status': 'ok', 'interrupt': interrupt}


    # -------------------------------------------------------------------------
    def do_shutdown(self, restart):
        """
        Shut down the IDL Kernel.
        >>> idl = IDL_kernel()
        >>> idl.do_shutdown(False)   # doctest: +ELLIPSIS
        {...}
        """
        try:
            self.log.debug("Shutting down IDL kernel...")
        except:
            pass
        return {'status': 'ok', 'execution_count': self.execution_count, 'restart': restart}


# -----------------------------------------------------------------------------
if __name__ == '__main__':
    from ipykernel.kernelapp import IPKernelApp
    IPKernelApp.launch_instance(kernel_class=IDL_kernel)
