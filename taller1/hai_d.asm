
./hai:     file format elf32-i386


Disassembly of section .init:

0804841c <_init>:
 804841c:	53                   	push   ebx
 804841d:	83 ec 08             	sub    esp,0x8
 8048420:	e8 00 00 00 00       	call   8048425 <_init+0x9>
 8048425:	5b                   	pop    ebx
 8048426:	81 c3 cf 1b 00 00    	add    ebx,0x1bcf
 804842c:	8b 83 fc ff ff ff    	mov    eax,DWORD PTR [ebx-0x4]
 8048432:	85 c0                	test   eax,eax
 8048434:	74 05                	je     804843b <_init+0x1f>
 8048436:	e8 75 00 00 00       	call   80484b0 <__gmon_start__@plt>
 804843b:	e8 80 01 00 00       	call   80485c0 <frame_dummy>
 8048440:	e8 bb 03 00 00       	call   8048800 <__do_global_ctors_aux>
 8048445:	83 c4 08             	add    esp,0x8
 8048448:	5b                   	pop    ebx
 8048449:	c3                   	ret    

Disassembly of section .plt:

08048450 <signal@plt-0x10>:
 8048450:	ff 35 f8 9f 04 08    	push   DWORD PTR ds:0x8049ff8
 8048456:	ff 25 fc 9f 04 08    	jmp    DWORD PTR ds:0x8049ffc
 804845c:	00 00                	add    BYTE PTR [eax],al
	...

08048460 <signal@plt>:
 8048460:	ff 25 00 a0 04 08    	jmp    DWORD PTR ds:0x804a000
 8048466:	68 00 00 00 00       	push   0x0
 804846b:	e9 e0 ff ff ff       	jmp    8048450 <_init+0x34>

08048470 <sleep@plt>:
 8048470:	ff 25 04 a0 04 08    	jmp    DWORD PTR ds:0x804a004
 8048476:	68 08 00 00 00       	push   0x8
 804847b:	e9 d0 ff ff ff       	jmp    8048450 <_init+0x34>

08048480 <wait@plt>:
 8048480:	ff 25 08 a0 04 08    	jmp    DWORD PTR ds:0x804a008
 8048486:	68 10 00 00 00       	push   0x10
 804848b:	e9 c0 ff ff ff       	jmp    8048450 <_init+0x34>

08048490 <execvp@plt>:
 8048490:	ff 25 0c a0 04 08    	jmp    DWORD PTR ds:0x804a00c
 8048496:	68 18 00 00 00       	push   0x18
 804849b:	e9 b0 ff ff ff       	jmp    8048450 <_init+0x34>

080484a0 <perror@plt>:
 80484a0:	ff 25 10 a0 04 08    	jmp    DWORD PTR ds:0x804a010
 80484a6:	68 20 00 00 00       	push   0x20
 80484ab:	e9 a0 ff ff ff       	jmp    8048450 <_init+0x34>

080484b0 <__gmon_start__@plt>:
 80484b0:	ff 25 14 a0 04 08    	jmp    DWORD PTR ds:0x804a014
 80484b6:	68 28 00 00 00       	push   0x28
 80484bb:	e9 90 ff ff ff       	jmp    8048450 <_init+0x34>

080484c0 <exit@plt>:
 80484c0:	ff 25 18 a0 04 08    	jmp    DWORD PTR ds:0x804a018
 80484c6:	68 30 00 00 00       	push   0x30
 80484cb:	e9 80 ff ff ff       	jmp    8048450 <_init+0x34>

080484d0 <kill@plt>:
 80484d0:	ff 25 1c a0 04 08    	jmp    DWORD PTR ds:0x804a01c
 80484d6:	68 38 00 00 00       	push   0x38
 80484db:	e9 70 ff ff ff       	jmp    8048450 <_init+0x34>

080484e0 <__libc_start_main@plt>:
 80484e0:	ff 25 20 a0 04 08    	jmp    DWORD PTR ds:0x804a020
 80484e6:	68 40 00 00 00       	push   0x40
 80484eb:	e9 60 ff ff ff       	jmp    8048450 <_init+0x34>

080484f0 <fprintf@plt>:
 80484f0:	ff 25 24 a0 04 08    	jmp    DWORD PTR ds:0x804a024
 80484f6:	68 48 00 00 00       	push   0x48
 80484fb:	e9 50 ff ff ff       	jmp    8048450 <_init+0x34>

08048500 <write@plt>:
 8048500:	ff 25 28 a0 04 08    	jmp    DWORD PTR ds:0x804a028
 8048506:	68 50 00 00 00       	push   0x50
 804850b:	e9 40 ff ff ff       	jmp    8048450 <_init+0x34>

08048510 <fork@plt>:
 8048510:	ff 25 2c a0 04 08    	jmp    DWORD PTR ds:0x804a02c
 8048516:	68 58 00 00 00       	push   0x58
 804851b:	e9 30 ff ff ff       	jmp    8048450 <_init+0x34>

08048520 <getppid@plt>:
 8048520:	ff 25 30 a0 04 08    	jmp    DWORD PTR ds:0x804a030
 8048526:	68 60 00 00 00       	push   0x60
 804852b:	e9 20 ff ff ff       	jmp    8048450 <_init+0x34>

Disassembly of section .text:

08048530 <_start>:
 8048530:	31 ed                	xor    ebp,ebp
 8048532:	5e                   	pop    esi
 8048533:	89 e1                	mov    ecx,esp
 8048535:	83 e4 f0             	and    esp,0xfffffff0
 8048538:	50                   	push   eax
 8048539:	54                   	push   esp
 804853a:	52                   	push   edx
 804853b:	68 f0 87 04 08       	push   0x80487f0
 8048540:	68 80 87 04 08       	push   0x8048780
 8048545:	51                   	push   ecx
 8048546:	56                   	push   esi
 8048547:	68 60 86 04 08       	push   0x8048660
 804854c:	e8 8f ff ff ff       	call   80484e0 <__libc_start_main@plt>
 8048551:	f4                   	hlt    
 8048552:	90                   	nop
 8048553:	90                   	nop
 8048554:	90                   	nop
 8048555:	90                   	nop
 8048556:	90                   	nop
 8048557:	90                   	nop
 8048558:	90                   	nop
 8048559:	90                   	nop
 804855a:	90                   	nop
 804855b:	90                   	nop
 804855c:	90                   	nop
 804855d:	90                   	nop
 804855e:	90                   	nop
 804855f:	90                   	nop

08048560 <__do_global_dtors_aux>:
 8048560:	55                   	push   ebp
 8048561:	89 e5                	mov    ebp,esp
 8048563:	53                   	push   ebx
 8048564:	83 ec 04             	sub    esp,0x4
 8048567:	80 3d 40 a0 04 08 00 	cmp    BYTE PTR ds:0x804a040,0x0
 804856e:	75 3f                	jne    80485af <__do_global_dtors_aux+0x4f>
 8048570:	a1 44 a0 04 08       	mov    eax,ds:0x804a044
 8048575:	bb 20 9f 04 08       	mov    ebx,0x8049f20
 804857a:	81 eb 1c 9f 04 08    	sub    ebx,0x8049f1c
 8048580:	c1 fb 02             	sar    ebx,0x2
 8048583:	83 eb 01             	sub    ebx,0x1
 8048586:	39 d8                	cmp    eax,ebx
 8048588:	73 1e                	jae    80485a8 <__do_global_dtors_aux+0x48>
 804858a:	8d b6 00 00 00 00    	lea    esi,[esi+0x0]
 8048590:	83 c0 01             	add    eax,0x1
 8048593:	a3 44 a0 04 08       	mov    ds:0x804a044,eax
 8048598:	ff 14 85 1c 9f 04 08 	call   DWORD PTR [eax*4+0x8049f1c]
 804859f:	a1 44 a0 04 08       	mov    eax,ds:0x804a044
 80485a4:	39 d8                	cmp    eax,ebx
 80485a6:	72 e8                	jb     8048590 <__do_global_dtors_aux+0x30>
 80485a8:	c6 05 40 a0 04 08 01 	mov    BYTE PTR ds:0x804a040,0x1
 80485af:	83 c4 04             	add    esp,0x4
 80485b2:	5b                   	pop    ebx
 80485b3:	5d                   	pop    ebp
 80485b4:	c3                   	ret    
 80485b5:	8d 74 26 00          	lea    esi,[esi+eiz*1+0x0]
 80485b9:	8d bc 27 00 00 00 00 	lea    edi,[edi+eiz*1+0x0]

080485c0 <frame_dummy>:
 80485c0:	55                   	push   ebp
 80485c1:	89 e5                	mov    ebp,esp
 80485c3:	83 ec 18             	sub    esp,0x18
 80485c6:	a1 24 9f 04 08       	mov    eax,ds:0x8049f24
 80485cb:	85 c0                	test   eax,eax
 80485cd:	74 12                	je     80485e1 <frame_dummy+0x21>
 80485cf:	b8 00 00 00 00       	mov    eax,0x0
 80485d4:	85 c0                	test   eax,eax
 80485d6:	74 09                	je     80485e1 <frame_dummy+0x21>
 80485d8:	c7 04 24 24 9f 04 08 	mov    DWORD PTR [esp],0x8049f24
 80485df:	ff d0                	call   eax
 80485e1:	c9                   	leave  
 80485e2:	c3                   	ret    
 80485e3:	90                   	nop

080485e4 <catcher_child>:
 80485e4:	55                   	push   ebp
 80485e5:	89 e5                	mov    ebp,esp
 80485e7:	83 ec 18             	sub    esp,0x18
 80485ea:	a1 48 a0 04 08       	mov    eax,ds:0x804a048
 80485ef:	83 c0 01             	add    eax,0x1
 80485f2:	a3 48 a0 04 08       	mov    ds:0x804a048,eax
 80485f7:	c7 44 24 08 07 00 00 	mov    DWORD PTR [esp+0x8],0x7
 80485fe:	00 
 80485ff:	c7 44 24 04 50 88 04 	mov    DWORD PTR [esp+0x4],0x8048850
 8048606:	08 
 8048607:	c7 04 24 01 00 00 00 	mov    DWORD PTR [esp],0x1
 804860e:	e8 ed fe ff ff       	call   8048500 <write@plt>
 8048613:	c9                   	leave  
 8048614:	c3                   	ret    

08048615 <catcher_parent>:
 8048615:	55                   	push   ebp
 8048616:	89 e5                	mov    ebp,esp
 8048618:	83 ec 28             	sub    esp,0x28
 804861b:	8d 45 f0             	lea    eax,[ebp-0x10]
 804861e:	89 04 24             	mov    DWORD PTR [esp],eax
 8048621:	e8 5a fe ff ff       	call   8048480 <wait@plt>
 8048626:	85 c0                	test   eax,eax
 8048628:	79 18                	jns    8048642 <catcher_parent+0x2d>
 804862a:	c7 04 24 58 88 04 08 	mov    DWORD PTR [esp],0x8048858
 8048631:	e8 6a fe ff ff       	call   80484a0 <perror@plt>
 8048636:	c7 04 24 01 00 00 00 	mov    DWORD PTR [esp],0x1
 804863d:	e8 7e fe ff ff       	call   80484c0 <exit@plt>
 8048642:	8b 45 f0             	mov    eax,DWORD PTR [ebp-0x10]
 8048645:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
 8048648:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 804864b:	83 e0 7f             	and    eax,0x7f
 804864e:	85 c0                	test   eax,eax
 8048650:	75 0c                	jne    804865e <catcher_parent+0x49>
 8048652:	c7 04 24 00 00 00 00 	mov    DWORD PTR [esp],0x0
 8048659:	e8 62 fe ff ff       	call   80484c0 <exit@plt>
 804865e:	c9                   	leave  
 804865f:	c3                   	ret    

08048660 <main>:
 8048660:	55                   	push   ebp
 8048661:	89 e5                	mov    ebp,esp
 8048663:	83 e4 f0             	and    esp,0xfffffff0
 8048666:	83 ec 20             	sub    esp,0x20
 8048669:	83 7d 08 01          	cmp    DWORD PTR [ebp+0x8],0x1
 804866d:	7f 2b                	jg     804869a <main+0x3a>
 804866f:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
 8048672:	8b 08                	mov    ecx,DWORD PTR [eax]
 8048674:	ba 60 88 04 08       	mov    edx,0x8048860
 8048679:	a1 3c a0 04 08       	mov    eax,ds:0x804a03c
 804867e:	89 4c 24 08          	mov    DWORD PTR [esp+0x8],ecx
 8048682:	89 54 24 04          	mov    DWORD PTR [esp+0x4],edx
 8048686:	89 04 24             	mov    DWORD PTR [esp],eax
 8048689:	e8 62 fe ff ff       	call   80484f0 <fprintf@plt>
 804868e:	c7 04 24 01 00 00 00 	mov    DWORD PTR [esp],0x1
 8048695:	e8 26 fe ff ff       	call   80484c0 <exit@plt>
 804869a:	e8 71 fe ff ff       	call   8048510 <fork@plt>
 804869f:	89 44 24 1c          	mov    DWORD PTR [esp+0x1c],eax
 80486a3:	83 7c 24 1c ff       	cmp    DWORD PTR [esp+0x1c],0xffffffff
 80486a8:	75 13                	jne    80486bd <main+0x5d>
 80486aa:	c7 04 24 83 88 04 08 	mov    DWORD PTR [esp],0x8048883
 80486b1:	e8 ea fd ff ff       	call   80484a0 <perror@plt>
 80486b6:	b8 01 00 00 00       	mov    eax,0x1
 80486bb:	c9                   	leave  
 80486bc:	c3                   	ret    
 80486bd:	83 7c 24 1c 00       	cmp    DWORD PTR [esp+0x1c],0x0
 80486c2:	75 66                	jne    804872a <main+0xca>
 80486c4:	c7 44 24 04 e4 85 04 	mov    DWORD PTR [esp+0x4],0x80485e4
 80486cb:	08 
 80486cc:	c7 04 24 17 00 00 00 	mov    DWORD PTR [esp],0x17
 80486d3:	e8 88 fd ff ff       	call   8048460 <signal@plt>
 80486d8:	90                   	nop
 80486d9:	a1 48 a0 04 08       	mov    eax,ds:0x804a048
 80486de:	83 f8 04             	cmp    eax,0x4
 80486e1:	7e f6                	jle    80486d9 <main+0x79>
 80486e3:	e8 38 fe ff ff       	call   8048520 <getppid@plt>
 80486e8:	c7 44 24 04 02 00 00 	mov    DWORD PTR [esp+0x4],0x2
 80486ef:	00 
 80486f0:	89 04 24             	mov    DWORD PTR [esp],eax
 80486f3:	e8 d8 fd ff ff       	call   80484d0 <kill@plt>
 80486f8:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
 80486fb:	8d 50 04             	lea    edx,[eax+0x4]
 80486fe:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
 8048701:	83 c0 04             	add    eax,0x4
 8048704:	8b 00                	mov    eax,DWORD PTR [eax]
 8048706:	89 54 24 04          	mov    DWORD PTR [esp+0x4],edx
 804870a:	89 04 24             	mov    DWORD PTR [esp],eax
 804870d:	e8 7e fd ff ff       	call   8048490 <execvp@plt>
 8048712:	c7 04 24 8e 88 04 08 	mov    DWORD PTR [esp],0x804888e
 8048719:	e8 82 fd ff ff       	call   80484a0 <perror@plt>
 804871e:	c7 04 24 01 00 00 00 	mov    DWORD PTR [esp],0x1
 8048725:	e8 96 fd ff ff       	call   80484c0 <exit@plt>
 804872a:	c7 44 24 04 15 86 04 	mov    DWORD PTR [esp+0x4],0x8048615
 8048731:	08 
 8048732:	c7 04 24 02 00 00 00 	mov    DWORD PTR [esp],0x2
 8048739:	e8 22 fd ff ff       	call   8048460 <signal@plt>
 804873e:	c7 04 24 01 00 00 00 	mov    DWORD PTR [esp],0x1
 8048745:	e8 26 fd ff ff       	call   8048470 <sleep@plt>
 804874a:	c7 44 24 08 05 00 00 	mov    DWORD PTR [esp+0x8],0x5
 8048751:	00 
 8048752:	c7 44 24 04 a4 88 04 	mov    DWORD PTR [esp+0x4],0x80488a4
 8048759:	08 
 804875a:	c7 04 24 01 00 00 00 	mov    DWORD PTR [esp],0x1
 8048761:	e8 9a fd ff ff       	call   8048500 <write@plt>
 8048766:	c7 44 24 04 17 00 00 	mov    DWORD PTR [esp+0x4],0x17
 804876d:	00 
 804876e:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
 8048772:	89 04 24             	mov    DWORD PTR [esp],eax
 8048775:	e8 56 fd ff ff       	call   80484d0 <kill@plt>
 804877a:	eb c2                	jmp    804873e <main+0xde>
 804877c:	90                   	nop
 804877d:	90                   	nop
 804877e:	90                   	nop
 804877f:	90                   	nop

08048780 <__libc_csu_init>:
 8048780:	55                   	push   ebp
 8048781:	57                   	push   edi
 8048782:	56                   	push   esi
 8048783:	53                   	push   ebx
 8048784:	e8 69 00 00 00       	call   80487f2 <__i686.get_pc_thunk.bx>
 8048789:	81 c3 6b 18 00 00    	add    ebx,0x186b
 804878f:	83 ec 1c             	sub    esp,0x1c
 8048792:	8b 6c 24 30          	mov    ebp,DWORD PTR [esp+0x30]
 8048796:	8d bb 20 ff ff ff    	lea    edi,[ebx-0xe0]
 804879c:	e8 7b fc ff ff       	call   804841c <_init>
 80487a1:	8d 83 20 ff ff ff    	lea    eax,[ebx-0xe0]
 80487a7:	29 c7                	sub    edi,eax
 80487a9:	c1 ff 02             	sar    edi,0x2
 80487ac:	85 ff                	test   edi,edi
 80487ae:	74 29                	je     80487d9 <__libc_csu_init+0x59>
 80487b0:	31 f6                	xor    esi,esi
 80487b2:	8d b6 00 00 00 00    	lea    esi,[esi+0x0]
 80487b8:	8b 44 24 38          	mov    eax,DWORD PTR [esp+0x38]
 80487bc:	89 2c 24             	mov    DWORD PTR [esp],ebp
 80487bf:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
 80487c3:	8b 44 24 34          	mov    eax,DWORD PTR [esp+0x34]
 80487c7:	89 44 24 04          	mov    DWORD PTR [esp+0x4],eax
 80487cb:	ff 94 b3 20 ff ff ff 	call   DWORD PTR [ebx+esi*4-0xe0]
 80487d2:	83 c6 01             	add    esi,0x1
 80487d5:	39 fe                	cmp    esi,edi
 80487d7:	75 df                	jne    80487b8 <__libc_csu_init+0x38>
 80487d9:	83 c4 1c             	add    esp,0x1c
 80487dc:	5b                   	pop    ebx
 80487dd:	5e                   	pop    esi
 80487de:	5f                   	pop    edi
 80487df:	5d                   	pop    ebp
 80487e0:	c3                   	ret    
 80487e1:	eb 0d                	jmp    80487f0 <__libc_csu_fini>
 80487e3:	90                   	nop
 80487e4:	90                   	nop
 80487e5:	90                   	nop
 80487e6:	90                   	nop
 80487e7:	90                   	nop
 80487e8:	90                   	nop
 80487e9:	90                   	nop
 80487ea:	90                   	nop
 80487eb:	90                   	nop
 80487ec:	90                   	nop
 80487ed:	90                   	nop
 80487ee:	90                   	nop
 80487ef:	90                   	nop

080487f0 <__libc_csu_fini>:
 80487f0:	f3 c3                	repz ret 

080487f2 <__i686.get_pc_thunk.bx>:
 80487f2:	8b 1c 24             	mov    ebx,DWORD PTR [esp]
 80487f5:	c3                   	ret    
 80487f6:	90                   	nop
 80487f7:	90                   	nop
 80487f8:	90                   	nop
 80487f9:	90                   	nop
 80487fa:	90                   	nop
 80487fb:	90                   	nop
 80487fc:	90                   	nop
 80487fd:	90                   	nop
 80487fe:	90                   	nop
 80487ff:	90                   	nop

08048800 <__do_global_ctors_aux>:
 8048800:	55                   	push   ebp
 8048801:	89 e5                	mov    ebp,esp
 8048803:	53                   	push   ebx
 8048804:	83 ec 04             	sub    esp,0x4
 8048807:	a1 14 9f 04 08       	mov    eax,ds:0x8049f14
 804880c:	83 f8 ff             	cmp    eax,0xffffffff
 804880f:	74 13                	je     8048824 <__do_global_ctors_aux+0x24>
 8048811:	bb 14 9f 04 08       	mov    ebx,0x8049f14
 8048816:	66 90                	xchg   ax,ax
 8048818:	83 eb 04             	sub    ebx,0x4
 804881b:	ff d0                	call   eax
 804881d:	8b 03                	mov    eax,DWORD PTR [ebx]
 804881f:	83 f8 ff             	cmp    eax,0xffffffff
 8048822:	75 f4                	jne    8048818 <__do_global_ctors_aux+0x18>
 8048824:	83 c4 04             	add    esp,0x4
 8048827:	5b                   	pop    ebx
 8048828:	5d                   	pop    ebp
 8048829:	c3                   	ret    
 804882a:	90                   	nop
 804882b:	90                   	nop

Disassembly of section .fini:

0804882c <_fini>:
 804882c:	53                   	push   ebx
 804882d:	83 ec 08             	sub    esp,0x8
 8048830:	e8 00 00 00 00       	call   8048835 <_fini+0x9>
 8048835:	5b                   	pop    ebx
 8048836:	81 c3 bf 17 00 00    	add    ebx,0x17bf
 804883c:	e8 1f fd ff ff       	call   8048560 <__do_global_dtors_aux>
 8048841:	83 c4 08             	add    esp,0x8
 8048844:	5b                   	pop    ebx
 8048845:	c3                   	ret    
