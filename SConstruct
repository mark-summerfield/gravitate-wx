import atexit
import contextlib
import os
import sys

WIN = sys.platform.startswith('win')

sources = ['gravitate.cpp', 'mainwindow.cpp']

if WIN:
    prefix = ' --prefix=C:/bin/wx31'
    wxconfig = 'C:/bin/wx-config.exe'
else:
    prefix = ''
    wxconfig = '/home/mark/opt/wxwidgets-3.1/buildgtk/wx-config'

ccflags = ['-std=gnu++11', '-Wall', '-O3', '-Wextra']

if WIN:
    env = Environment(CCFLAGS=ccflags, tools=['mingw'])
    env.Append(LINKFLAGS='-mwindows') # Comment out if debugging on Windows
else:
    env = Environment(CCFLAGS=ccflags)
env.ParseConfig(f'{wxconfig}{prefix} --libs --cxxflags')
app = env.Program('hellowx', sources)


def run_at_exit(exe):
    if os.path.exists(exe):
        os.system(exe)

AddOption('--run', dest='run', action='store_true')
if GetOption('run'):
    exe = app[0].abspath
    with contextlib.suppress(FileNotFoundError):
        os.remove(exe)
    atexit.register(lambda: run_at_exit(exe))
