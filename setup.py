from setuptools import setup
from Cython.Build import cythonize
import Cython.Compiler.Options
Cython.Compiler.Options.annotate = True


setup(
    name='Solver',
    ext_modules=cythonize('solver.pyx', annotate=True),
    zip_safe=False,
)

