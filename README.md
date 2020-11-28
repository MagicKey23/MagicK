# MagicK


# instruction:


### Depend on architect system. You might need to run cc or gcc to compile



## WINDOW

To compile the code you need to install MINGW

http://www.mingw.org/

 pick at least mingw32-base and msys-base
 
 Then follow this instruction:
 
 https://www.computerhope.com/issues/ch000549.htm




## LINUX and MAC


1. Install editline library


--FOR ERROR--


"fatal error: editline/readline.h: No such file or directory #include <editline/readline.h>"


On Mac the editline library comes with Command Line Tools. You may still get an error about the history header not being found. In this case remove the line #include <editline/history.h>, as this header may not be required.

On Linux you can install editline with sudo apt-get install libedit-dev. On Fedora you can use the command su -c "yum install libedit-dev*"

2.LINKING Process

--SECOND ERROR--

"undefined reference to `readline'"
"undefined reference to `add_history'"

This means that you haven't linked your program to editline. This linking process allows the compiler to directly embed calls to editline in your program. 
You can make it link by adding the flag -ledit to your compile command, just before the output flag.

run this command for linking

gcc -std=c99 -Wall prompt_mac_linux.c -ledit -o prompt_mac_linux


-----------

For class:

The language program using disjunction to create parser for the noun and adjection. Later, it use conjunction rule to joined the adjective and noun together to make a phrase.



# How to run the executable file

PREFERRED WINDOW 

1. OPEN your CMD 
2. NAVIGATE TO THE FILE DIRECTORY
3. type "parse.exe"

Example usage:

|Symbol|
|---------|
|list, head, tail, join, eval|		         
|+ ,-,*,/,% | 											 
|add, sub,mul,mod|  
         
		 
**list 1 2 3 4 **<=> This would create a list {1 2 3 4}

**eval {head (list 1 2 3 4)} ** <=> Grab the first index in the list

**eval (tail {tail tail {5 6 7}})** <=> remove first index return last two index in the list

**head {1, 2 ,3}** <=> return the head

**tail {1, 2 ,3 } **<=> return last 2 index, {2,3}

**tail (tail {1, 2 ,3}** <=> return last index, {3}

**+ 1 2 3 <=> 1 + 2 + 3**

**+ 1 (* 7 5) 3 = 1 + (7*5) + 3**

** eval (head {(+ 1 2) (+ 10 20)}) ** <=> grab the first list perform calculation

** eval (tail {(+ 1 2) (+ 10 20)}) ** <=> grab the last list perform calculation


SPECIAL THANKS TO DANIEL HOLDEN for create awesome parser combination tool




