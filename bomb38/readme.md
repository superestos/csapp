
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