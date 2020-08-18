import atexit
import contextlib
import os
import pathlib
import subprocess
import sys


WIN = sys.platform.startswith('win')

appname = 'Gravitate'
sources = [Glob('*.cpp')]

if WIN:
    prefix = ' --prefix=C:/bin/wx31'
    wxconfig = 'C:/bin/wx-config.exe'
    rcedit = 'C:/bin/rcedit.exe'
else:
    prefix = ''
    wxconfig = '/home/mark/opt/wxwidgets-3.1/buildgtk/wx-config'

ccflags = ['-Wall', '-O3', '-Wextra']

if WIN:
    ccflags.append('-m64')
    env = Environment(CCFLAGS=ccflags, tools=['mingw'])
    env.Append(LINKFLAGS=['-m64', '-mwindows'])
else:
    env = Environment(CCFLAGS=ccflags)
env.ParseConfig(f'{wxconfig}{prefix} --libs --cxxflags')
app = env.Program(appname, sources)


def run_at_exit(exe):
    if os.path.exists(exe):
        if WIN:
            icon = pathlib.Path('images/icon.ico')
            if not icon.exists():
                icon = pathlib.Path(f'images/{appname.lower()}.ico')
            if icon.exists():
                args = [rcedit, f'{appname}.exe', '--set-icon', str(icon)]
                if subprocess.run(args).returncode != 0:
                    print('failed to add icon')
        os.system(exe)

AddOption('--run', dest='run', action='store_true')
if GetOption('run'):
    exe = app[0].abspath
    with contextlib.suppress(FileNotFoundError):
        os.remove(exe)
    atexit.register(lambda: run_at_exit(exe))
