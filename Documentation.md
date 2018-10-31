# 6502 Architecture
-------------------
## Processor Structure
CPU 8-bits
RAM : 64ko
$0000 - $00ff : Zero Page : ram réservé
$0100 - $01ff : Stack
$fffa - $ffff : Reserved for:
	-Non-maskable interrupt handler $fffa - $fffb
	-Power on reset location $fffc - $fffd
	-brk interrupt request handler $fffe - $ffff
#### Registres

- Program Counter:
	> Registre 16 bits
	> Pointe vers l'instruction suivante
- Stack pointer:
	> 256 bit stack: $0100 - $01ff
	> Pinteur 8 bits, indique les bits faibles de la position sur la stack
	> On décrémente le pointeur de stack quand on ajoute des valeurs à la stack, et on l'incrémente si on en enlève
	> Le CPU ne peut pas détecter si il y a un overflow
- Accumulateur:
	> Registre 8-bits utilisé pour les op logiques et arithmétiques

- Registre d'index X:
	> Registre 8 bits
	> Utilisé pour les compteurs, et les offset pour accéder à la mémoire.
	> Le registre X peut-être utilisé pour récupérer une copie du pointeur de Stack, ou modifier sa valeur

- Registre d'index Y:
	> Registre 8-bits
	> Idem que X, mais sans fonctions spéciales


#### Status du Processeur

Le CPU dispose d'un certain nombres de Flags pour indiquer le résultat des opérations précédentes.
Ces flags sont contenus dans un registre particulier.
- Carry Flag
	> Set si la dernère opération à causé un overflow ou un underflow (Arithmétique ou Shifts)
- Zero Flag
	> Set si le résultat de la dernière opération est zéro
- Interrupt Disable
	> Set si le programme a exécuter un SEI (Set interrupt Disable).
	> Le processeur ne répondra pas aux interruptions des appareils tant que le flag ne sera pas nul (CLD : Clear Interrupt Disable)
- Decimal mode
	> Tant que le flag est set > Le processeur fonctionne en BCD
	> On peut l'activer explicitement avec SED / CLD
- Break Command
	> Flag Set quand une instruction BRK est exécuté et qu'une interruption a été générée
- Overflow Flag
	> Si le résultat de l'opération n'as pas un complément à 2 viable
	> Déterminé en regardant le bit de Carry entre le bit 6 et le bit 7, et entre le bit 7 et le carry bit
- Negative Flag
	> Si le résultat de l'opération voit le 8ème bit mis à 1 -> Se
## Addressing Mode

### Immediat
On charge directement le registre avec la valeur suivante

### Zero Page
On travaille sur les 256 premiers octets de la RAM
 -> Pas besoin d'utiliser le 1er bit de l'addresse
 -> Instruction sur 2 octets
### Absolu
On indique une addresse complète derrière l'instruction sur 16bits
 -> On modifie la valeur 8bit de l'addresse pointée (sur 16 bits)

### Absolu indirect
 -> Utilisé seulement par l'instruction JMP
 La valeur utilisée est une addresse mémoire qui contient à son tour une autre addresse mémoire (sur 16bits)

### Indexé Indirect
La valeur indiquée est une valeur sur 16 bits.
Le contenu du registre X est ensuite ajouté à cette valeur. On obtient une addresse, et à cette addresse, on trouve une nouvelle adresse, qui est celle de la valeur à modifier.

### Indirect Indexé
La valeur indiqué est une valeur sur 16 bits, qui corespond à une addresse dans laquelle on trouve une nouvelle valeur sur 16 bits. On ajoute le contenu de Y à cette valeur, on obtient alors l'adresse de la valeur 8bit à modifier.

### Relatif

Seules es instructions conditionnelles permettent l'adressage relatif. Indique le nombre d'octet à passer pour retrouver la prochaine instruction.
si val in [0:127] > Saut en avant
si val in [0:127] > Saut en arrière


## [Opcodes][http://6502.org/tutorials/6502opcodes.html]

### ADC

### AND

### ASL

### BIT

### Branch

### BRK

### CMP

### CPX

### CPY

### DEC

### EOR

### Flag Instructions

### INC

### JMP

### JSR

### LDA

### LDX

### LDY

### LSR

### NOP

### ORA

### Registers instructions

### ROL

### ROR

### RTI

### RTS

### SBC

### STA

### Stack Instruction

### STX

### STY

# Nes Mappers
-------------



# Audio Processing unit
-----------------------
