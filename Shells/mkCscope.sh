find . -name "*.h" -o -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.cxx" -o -name "*.java" > cscope.files  
cscope -b 
ctags -R 
