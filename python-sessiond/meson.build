python = import('python')

python_sessiond_srcs = [
  'sessiond.py'
]

py_installation = python.find_installation('python3', required: true)
if not py_installation.found()
  error('No python installation found')
endif

py_installation.install_sources(python_sessiond_srcs)
