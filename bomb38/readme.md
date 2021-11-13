
## phase 1

 08048b80 <phase_1>:
 8048b80:	55                   	push   %ebp
 8048b81:	89 e5                	mov    %esp,%ebp
 8048b83:	83 ec 08             	sub    $0x8,%esp
 8048b86:	c7 44 24 04 48 99 04 	movl   $0x8049948,0x4(%esp)
 8048b8d:	08 
 8048b8e:	8b 45 08             	mov    0x8(%ebp),%eax
 8048b91:	89 04 24             	mov    %eax,(%esp)
 8048b94:	e8 f6 04 00 00       	call   804908f <strings_not_equal>

(gdb) x /s 0x8049948
0x8049948:      "Public speaking is very easy."

## phase 2

 8048bb7:	e8 40 04 00 00       	call   8048ffc <read_six_numbers>
 8048bbc:	8b 45 e4             	mov    -0x1c(%ebp),%eax
 8048bbf:	83 f8 01             	cmp    $0x1,%eax
 8048bc2:	74 05                	je     8048bc9 <phase_2+0x25>

%eax = 1 = *(ebp - 0x1c)

 8048bc9:	c7 45 fc 01 00 00 00 	movl   $0x1,-0x4(%ebp)
 8048bd0:	eb 22                	jmp    8048bf4 <phase_2+0x50>

 8048bd2:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048bd5:	8b 4c 85 e4          	mov    -0x1c(%ebp,%eax,4),%ecx
 8048bd9:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048bdc:	48                   	dec    %eax
 8048bdd:	8b 54 85 e4          	mov    -0x1c(%ebp,%eax,4),%edx
 8048be1:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048be4:	40                   	inc    %eax
 8048be5:	0f af c2             	imul   %edx,%eax
 8048be8:	39 c1                	cmp    %eax,%ecx
 8048bea:	74 05                	je     8048bf1 <phase_2+0x4d>
 8048bec:	e8 65 0a 00 00       	call   8049656 <explode_bomb>
 8048bf1:	ff 45 fc             	incl   -0x4(%ebp)
 8048bf4:	83 7d fc 05          	cmpl   $0x5,-0x4(%ebp)
 8048bf8:	7e d8                	jle    8048bd2 <phase_2+0x2e>

init *(%ebp - 4) = 1; loop until *(%ebp - 4) == 5; *(%ebp - 4)++. Denote it as i

%ecx = *(%ebp + 4 * i - 0x1c)
%edx = *(%ebp + 4 * (i - 1) - 0x1c)
%eax = (i + 1) * %edx

%eax == %ecx

1 2 6 24 120 720

## phase 3

 8048bff:	83 ec 28             	sub    $0x28,%esp
 8048c02:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%ebp)
 8048c09:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%ebp)
 8048c10:	8d 45 f0             	lea    -0x10(%ebp),%eax
 8048c13:	89 44 24 0c          	mov    %eax,0xc(%esp)
 8048c17:	8d 45 f4             	lea    -0xc(%ebp),%eax
 8048c1a:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048c1e:	c7 44 24 04 66 99 04 	movl   $0x8049966,0x4(%esp)
 8048c25:	08 
 8048c26:	8b 45 08             	mov    0x8(%ebp),%eax
 8048c29:	89 04 24             	mov    %eax,(%esp)
 8048c2c:	e8 37 fc ff ff       	call   8048868 <sscanf@plt>

(gdb) x /s 0x8049966
0x8049966:      "%d %d"

scanf("%d %d", &a, &b);

a in *(%ebp - 0xc) b in *(%ebp - 0x10)

 8048c3f:	8b 45 f4             	mov    -0xc(%ebp),%eax
 8048c42:	89 45 ec             	mov    %eax,-0x14(%ebp)
 8048c45:	83 7d ec 07          	cmpl   $0x7,-0x14(%ebp)
 8048c49:	77 54                	ja     8048c9f <phase_3+0xa3>

 8048c9f:	e8 b2 09 00 00       	call   8049656 <explode_bomb>

*(%ebp - 0x14) = a < 7

 8048c4b:	8b 55 ec             	mov    -0x14(%ebp),%edx
 8048c4e:	8b 04 95 6c 99 04 08 	mov    0x804996c(,%edx,4),%eax
 8048c55:	ff e0                	jmp    *%eax

jump to 0x804996c + 4 * a

 8048c57:	c7 45 f8 f0 01 00 00 	movl   $0x1f0,-0x8(%ebp)
 8048c5e:	eb 44                	jmp    8048ca4 <phase_3+0xa8>

 8048ca4:	8b 45 f0             	mov    -0x10(%ebp),%eax
 8048ca7:	39 45 f8             	cmp    %eax,-0x8(%ebp)
 8048caa:	74 05                	je     8048cb1 <phase_3+0xb5>

b == *(%ebp - 8)

(gdb) p /x *(int *)(0x804996c)
$16 = 0x8048c57
(gdb) p 0x1f0
$18 = 496

a = 0 b = 496

## phase 4

scanf("%s", &a);
a > 0
func4(a) == 0x37 == 55

func4(a) {
    if (a > 1) {
        b = func(a - 1)
        a -= 2
        b += func4(a)

        return b
    }
    return 1
}

1 2 3 5 8 13 21 34 55
func4(9) == 55

## phase 5

 8048d4f:	e8 11 03 00 00       	call   8049065 <string_length>
 8048d54:	89 45 fc             	mov    %eax,-0x4(%ebp)
 8048d57:	83 7d fc 06          	cmpl   $0x6,-0x4(%ebp)
 8048d5b:	74 05                	je     8048d62 <phase_5+0x1f>
 8048d5d:	e8 f4 08 00 00       	call   8049656 <explode_bomb>

strlen(input) == 6

 8048d62:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%ebp)
 8048d69:	eb 20                	jmp    8048d8b <phase_5+0x48>

 8048d6b:	8b 55 f8             	mov    -0x8(%ebp),%edx
 8048d6e:	8b 45 f8             	mov    -0x8(%ebp),%eax
 8048d71:	03 45 08             	add    0x8(%ebp),%eax
 8048d74:	0f b6 00             	movzbl (%eax),%eax
 8048d77:	0f be c0             	movsbl %al,%eax
 8048d7a:	83 e0 0f             	and    $0xf,%eax
 8048d7d:	0f b6 80 c0 a5 04 08 	movzbl 0x804a5c0(%eax),%eax
 8048d84:	88 44 15 f1          	mov    %al,-0xf(%ebp,%edx,1)
 8048d88:	ff 45 f8             	incl   -0x8(%ebp)
 8048d8b:	83 7d f8 05          	cmpl   $0x5,-0x8(%ebp)
 8048d8f:	7e da                	jle    8048d6b <phase_5+0x28>

i in *(%ebp - 0x8)
for (i = 0; i <= 5; i++) {
    a = input[i]
    *(%ebp + i - 0xf) = *(0x804a5c0 + (a & 0xf))
}

 8048d91:	c6 45 f7 00          	movb   $0x0,-0x9(%ebp)
 8048d95:	c7 44 24 04 8f 99 04 	movl   $0x804998f,0x4(%esp)
 8048d9c:	08 
 8048d9d:	8d 45 f1             	lea    -0xf(%ebp),%eax
 8048da0:	89 04 24             	mov    %eax,(%esp)
 8048da3:	e8 e7 02 00 00       	call   804908f <strings_not_equal>
 8048da8:	85 c0                	test   %eax,%eax
 8048daa:	74 05                	je     8048db1 <phase_5+0x6e>
 8048dac:	e8 a5 08 00 00       	call   8049656 <explode_bomb>
 8048db1:	c9                   	leave  
 8048db2:	c3                   	ret 

compare input and *(%ebp - 0xf)

(gdb) x /s 0x804998f
0x804998f:      "giants"

(gdb) x /s 0x804a5c0
0x804a5c0 <array.2486>: "isrveawhobpnutfg\323\003"

select 15 0 5 11 13 1
o`ekma

## phase 6

(gdb) x 0x804a63c
0x804a63c <node0>:      0x00000000
(gdb) x 0x804a63c + 8
0x804a644 <node0+8>:    0x0804a630
(gdb) x 0x804a630
0x804a630 <node1>:      0x000000dc
(gdb) x 0x804a630 + 8
0x804a638 <node1+8>:    0x0804a624
(gdb) x 0x804a624
0x804a624 <node2>:      0x000000a7
(gdb) x 0x804a624 + 8
0x804a62c <node2+8>:    0x0804a618
(gdb) x 0x804a618
0x804a618 <node3>:      0x0000022e
(gdb) x 0x804a618 + 8
0x804a620 <node3+8>:    0x0804a60c
(gdb) x 0x804a60c
0x804a60c <node4>:      0x000000ac
(gdb) x 0x804a60c + 8
0x804a614 <node4+8>:    0x0804a600
(gdb) x 0x804a600
0x804a600 <node5>:      0x00000358
(gdb) x 0x804a600 + 8
0x804a608 <node5+8>:    0x0804a5f4
(gdb) x 0x804a5f4
0x804a5f4 <node6>:      0x0000032c
(gdb) x 0x804a5f4 + 8
0x804a5fc <node6+8>:    0x0804a5e8
(gdb) x 0x804a5e8
0x804a5e8 <node7>:      0x000001b8
(gdb) x 0x804a5e8 + 8
0x804a5f0 <node7+8>:    0x0804a5dc
(gdb) x 0x804a5dc
0x804a5dc <node8>:      0x000000c8
(gdb) x 0x804a5dc + 8
0x804a5e4 <node8+8>:    0x0804a5d0
(gdb) x 0x804a5d0
0x804a5d0 <node9>:      0x000003d3
(gdb) x 0x804a5d0 + 8
0x804a5d8 <node9+8>:    0x00000000

 8048e4d:	c7 45 f8 3c a6 04 08 	movl   $0x804a63c,-0x8(%ebp)
 8048e54:	8b 45 08             	mov    0x8(%ebp),%eax
 8048e57:	89 04 24             	mov    %eax,(%esp)
 8048e5a:	e8 f9 f9 ff ff       	call   8048858 <atoi@plt>
 8048e5f:	89 c2                	mov    %eax,%edx
 8048e61:	8b 45 f8             	mov    -0x8(%ebp),%eax
 8048e64:	89 10                	mov    %edx,(%eax)
 8048e66:	8b 45 f8             	mov    -0x8(%ebp),%eax
 8048e69:	89 04 24             	mov    %eax,(%esp)
 8048e6c:	e8 42 ff ff ff       	call   8048db3 <fun6>

node0->val = atoi(input)
p = fun6(node0)

 8048e71:	89 45 f8             	mov    %eax,-0x8(%ebp)
 8048e74:	8b 45 f8             	mov    -0x8(%ebp),%eax
 8048e77:	89 45 fc             	mov    %eax,-0x4(%ebp)
 8048e7a:	c7 45 f4 01 00 00 00 	movl   $0x1,-0xc(%ebp)
 8048e81:	eb 0c                	jmp    8048e8f <phase_6+0x48>

 8048e83:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048e86:	8b 40 08             	mov    0x8(%eax),%eax
 8048e89:	89 45 fc             	mov    %eax,-0x4(%ebp)
 8048e8c:	ff 45 f4             	incl   -0xc(%ebp)
 8048e8f:	83 7d f4 04          	cmpl   $0x4,-0xc(%ebp)
 8048e93:	7e ee                	jle    8048e83 <phase_6+0x3c>

for (i = 1; i <= 4; i++) {
    p = p->next
}

 8048e95:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048e98:	8b 10                	mov    (%eax),%edx
 8048e9a:	a1 3c a6 04 08       	mov    0x804a63c,%eax
 8048e9f:	39 c2                	cmp    %eax,%edx
 8048ea1:	74 05                	je     8048ea8 <phase_6+0x61>
 8048ea3:	e8 ae 07 00 00       	call   8049656 <explode_bomb>
 8048ea8:	c9                   	leave  
 8048ea9:	c3                   	ret  

p == 0x804a63c

fun6:

 8048e3c:	83 7d f4 00          	cmpl   $0x0,-0xc(%ebp)
 8048e40:	75 98                	jne    8048dda <fun6+0x27>


-0x10(%ebp) head
-0xc(%ebp)  p
-0x8(%ebp)  prev
-0x4(%ebp)  q

head = input
p = input->next
head->next = NULL
goto L7

L1:
q = head
prev = head
goto L3

L2:
prev = q
q = q->next
L3:
if (q == NULL) {
    goto L4
}
if (q->val < p->val) {
    goto L2
}

L4:
if (prev == q) {
    goto L5
}
prev->next = p
goto L6

L5:
head = p

L6:
prev = p->next
p->next = q
p = prev

L7:
if (p != NULL) {
    goto L1
}

return head

fun6(input) {
    head = input
    p = input->next
    input->next = NULL

    while (p != NULL) {
        q = head
        prev = head

        while (q->val < p->val) {
            prev = q
            q = q->next

            if (q == NULL) {
                break
            }
        }

        if (prev == q) {
            head = p
        }
        else {
            prev->next = p
        }

        prev = p->next
        p->next = q
        p = prev
    }

    return head
}

0dc 0a7 22e 0ac 358 32c 1b8 3d3 0c8 3d3
sorted
0a7 0ac 0c8 0dc 1b8 22e

we choose 0x200, which between 1b8 and 22e



