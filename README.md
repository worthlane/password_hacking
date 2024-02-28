# password_hacking

My friend made a program, that imitates password checking and access for some data. It has 2 vulnerabilities, that I need to find. Now I'm going to describe the sequence of my actions, when I tried to hack this program.

## First weakness (easier one) "god mode"

First of all I tried to analyse program structure.

![](https://github.com/worthlane/password_hacking/blob/main/main.jpg)

We see that program calls for two main functions (0029 and 000B), and after that terminates.  
Let's have a look at the end of program:

![](https://github.com/worthlane/password_hacking/blob/main/prompts.jpg)

Seems that we've found output prompts addresses. Program asks us to input password. Then it draws 40x15 frame, that shows us if we have permission or not.
Also we can see a lot of zeros. I suppose, that it can be a buffer, which contains our input.  
Lets remember these important addreses, it may be helpful in the future. 

```
SUCCESS_PROMPT = 02E2h   // we are adding 100 because of default program shift in the start
FAILURE_PROMPT = 0300h
```

Obviously, we can find these addresses in the code. I saw them in the procedure with 000B start addres (mentioned before)

![](https://github.com/worthlane/password_hacking/blob/main/adrss.jpg)

This function calls for 0068 procedure. 0068 procedure use videomemory (0b800h). I suppose that that function use prompt and then draws frame. But you may have not noticed something very interesting in the start of this function.

```
cmp word ptr [0082], 3837   ; compare [0082] and "78"
```

What is this command doing? And why is it using commang line arguments adress (082h). If [0082] == 78 program will always save SUCCESS_PROMPT in si. Let's try to enter 78 in command line:

![](https://github.com/worthlane/password_hacking/blob/main/success.jpg)

I'm sure that this password is not the correct one, but we've passed verification and got permission.
