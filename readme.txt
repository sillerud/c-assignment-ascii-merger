My solution is based on 3 files, a merge.c which has a header merge.h and
main.c which makes use of the logic in merge.c. I’ve chosen to create my
own struct to avoid a triple pointer and because all parts of the result
is used all all times, also I wanted to be able to use the return value to
give feedback if the function was ran successfully. I chose to parse the
filenames for the parts with a regex, this is because it’s a flexible way
to do it and if I want it to make it work on another format I can just
change the regex. The strings returned is based on a char pointer pointer
which works as a array of strings, I’ve made it so the file read method
reads directly into the char pointer pointer.


When compiled you can print it to stdout using
./merge /path/to/folder
if you want to output it to a file without using piping you can do
./merge /path/to/folder /path/to/output.txt
or if you’re in the same folder just do
./merge folder
./merge folder output.txt


To compile you can use the command gcc -o merge merge.c merge.h main.c
