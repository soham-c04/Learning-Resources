file="$1"

bison -d -t "$file.y"
flex "$file.l"
g++ "${file}.tab.c" lex.yy.c && ./a.out
