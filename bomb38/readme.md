
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
