IDL Kernel README


Files:
    idl_kernel.py
    kernels/IDL/kernel.json


Uses:
    Python wrappers for IPython (Jupyter) Notebook.  Allows IDL code to be run in the notebooks.


Installation:
    idl_kernel.py can be moved anywhere so long as it is on Python's path.
    kernels/IDL/kernel.json should be placed in one of the following locations:

    Unix System installation:
        - /usr/share/jupyter/kernels/IDL
        - /usr/local/share/jupyter/kernels/IDL

    Linux single user installation:
        - ~/.local/share/jupyter/kernels/IDL

    Mac single user installation:
        - ~/Library/Jupyter/kernels/IDL

    Windows System installation:
        - %PROGRAMDATA%\jupyter\kernels\IDL

    Environment Variable:
        Instead of copying the kernel.json file, you can define an environment variable:
           JUPYTER_PATH = <IDL_DIR>/lib/bridges
        where <IDL_DIR> should be changed to your IDL installation directory.

Note:
    kernels/IDL/kernel.json has an item under argv called python.
    This string is directly executed by the system.
    If python isn't on the path, the IDL kernel will not load.
    Conversely, if IDL kernel ever needs a
    specific version of python, specifying the full path in argv
    will allow the kernel to pick and choose which
    python to use.

Additional Information:
    See ipython.org and jupyter.org
