file="$1"

bison -d "$file.y"
flex "$file.l"
g++ "${file}.tab.c" lex.yy.c && ./a.out
