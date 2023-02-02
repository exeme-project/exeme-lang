rm -rf doxygen

doxygen doxyfile

clear

cd doxygen/html

python -m http.server 8000