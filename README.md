# password_hacking

I am trying to access a friend's program, but it asks me for a password that I do not know💀

![](https://github.com/worthlane/password_hacking/blob/main/img/deny.jpg)

I'm too angry, so I will hack it😈

## First vulnerability (easier one) "god mode"

First of all I tried to analyze program structure.

![](https://github.com/worthlane/password_hacking/blob/main/img/main.jpg)

We see that program calls for <a name="funcs">two main functions"</a> (0029 and 000B), and after that terminates.  
Let's have a look at the end of program:

![](https://github.com/worthlane/password_hacking/blob/main/img/prompts.jpg)

Seems that we've found output prompts addresses. Program asks us to input password. Then it draws 40x15 frame, that shows us if we have permission or not.
Also we can see a lot of zeros. I suppose, that it can be a buffer, which contains our input.  

```
&SUCCESS_PROMPT = 02E2h   // we are adding 100 because of default program shift in the start
&FAILURE_PROMPT = 0300h
```

Obviously, we can find these addresses in the code. I saw them in the procedure with 000B start address (mentioned before)

![](https://github.com/worthlane/password_hacking/blob/main/img/adrss.jpg)

This function calls for 0068 procedure, that pushes videomemory segment in registers. I suppose that that function use prompt and then draws frame. But you may have not noticed something very interesting in the start of this function.

```
cmp word ptr [0082], 3837   ; compare [0082] and "78"
```

What does this command? And why is it using commang line arguments adress (082h). If [0082] == 78 program will always save SUCCESS_PROMPT in SI register. Let's try to enter 78 in command line:

![](https://github.com/worthlane/password_hacking/blob/main/img/success.jpg)

I'm sure that this password is not the correct one, but we passed verification and got the permission.

## Second vulnerability (harder one) "buffer overflow"

### Pre-hack information

In the [beginning](#funcs) I've noticed, that we have in fact this program works by calling two important functions. We already know that procedure 000B is deciding to give us access or not, so, obviously, second important procedure (0029) reads input from keyboard. Let's check keyboard input using TurboDebugger.

![](https://github.com/worthlane/password_hacking/blob/main/img/input.jpg)

So, now we know where program saves input. Also I've noticed that two bytes after this string change depending on the length of the entered string. Later I saw, that this is a counter, that limits amount of symbols we can enter (Keep an eye on the address [032F]):

![](https://github.com/worthlane/password_hacking/blob/main/img/counter.jpg)

By buffer overflowing we can change this counter in the way we want. So, now I can describe my plan to hack this program.

### Hacking

Main idea: We can use buffer overflow to reach stack. Stack uses some elements as return-addreses, so, we can change them and return in the any program place we want, avoiding any checks. While we are getting keyboard input, stack looks like this:

![](https://github.com/worthlane/password_hacking/blob/main/img/stack.jpg)

Program wants us to return on the 103h address, where it starts to compare our password and the correct one. Why wouldn't we return in the other place? For example at 113h, where we directly draw frame with success prompt.

![](https://github.com/worthlane/password_hacking/blob/main/img/better_addr.jpg)

Now we meet a problem. We see return, but we already popped from stack 0113h addres. In this situation stack points at the FFFE address, so, we can fill this memory cell with address of terminate function (0106h).  

So, how can we do such a buffer overflow? We must enter very long password, we will generate it by C program:  

1) First of all we skip all symbols in input buffer, to go beyond borders.

```
for (int i = 0; i < BUF_LEN; i++)                // overflowing buffer
        fputc('a', fp);
```

2) Then we place FFFF in the counter, to remove limit in symbols input.
```
fputc(0xFF, fp);
fputc(0xFF, fp);
```

3) After that we place a lot of symbols to reach stack address.
```
for (int i = 0; i < ~0xFFFF; i++)             // trying to reach stack
        fputc('A', fp);
```

4) And in the end we place return addresses and string end symbol.
```
fputc(0x13, fp);                            // first return adress
fputc(0x01, fp);

fputc(0x06, fp);                            // second return adress
fputc(0x01, fp);

fputc('$', fp);                             // end of input
```

Now we have very long password in the file. Lets try this password by input from file

![](https://github.com/worthlane/password_hacking/blob/main/img/success2.jpg)



## Cracking

We can abuse first vulnerability to write program, that can crack this password system. Non-cracked program looks like:

![](https://github.com/worthlane/password_hacking/blob/main/img/noncracked.jpg)

I have already mentioned, that this program always gives permission if we have "78" in command line. Lets simulate, that we always have this number. So, we need to skip conditional jump (011h - 012h) and we can also skip command line comparison (00Bh - 010h). We can replace these instructions with NOP. Then SI register will always recieve SUCCESS_PROMPT adress and we will pass through password checking. To avoid shift in other jumps in this program, we have to put as many NOP's as bytes we have deleted. After this removement, we recieve program, that gives us permission with any written password:

![](https://github.com/worthlane/password_hacking/blob/main/img/aftercracked.jpg)
