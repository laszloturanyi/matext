from subprocess import Popen, PIPE
import os

core_dir = os.path.dirname(os.path.realpath(__file__))
#core_dir += '\matext_core'

process = Popen(['python', 'setup.py', 'build_ext', '--inplace'], cwd=core_dir, stdout=PIPE, stderr=PIPE)
stdout, stderr = process.communicate()

print(stdout)
print(stderr)