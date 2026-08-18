![C23](https://img.shields.io/badge/C-C23-blue?style=flat-square)
![SDL2](https://img.shields.io/badge/SDL2-required-green?style=flat-square)
![SDL2_ttf](https://img.shields.io/badge/SDL2__ttf-required-green?style=flat-square)

![version](https://img.shields.io/badge/version-v0.1.9-orange?style=flat-square)
![extension](https://img.shields.io/badge/extension-v0.2.2-orange?style=flat-square)

# Cltext

markup language

## demo:

https://github.com/user-attachments/assets/34cc6d67-3ca6-4160-9af5-2f3584ac2ee2

code:
```clt
<txtcolor c="#000000">
<bgcolor c="#ff0000">
<title name="example">
<reverse>there is this</reverse> <bold><wave a=2>thingy</wave></bold> called a <bold>particle</bold>
<shake a=1>it</shake> is <spin a=1>very</spin> <tilt><small>small.</small></tilt>
<?==
 comment 
==?>
<bold><glitch>it</glitch> is <wave a=0.5>part <big>of</big> an <bounce a=0.5><color c="#0000ff">atom</color></bounce></wave>.</bold>
<?== other comment ==?>
```

## Installation and file extension:

1. clone the repo to `C:\Cltext`
2. add `C:\Cltext` to **PATH**
    * if the SDL2 DLLs [egg. `SDL2_ttf.dll`] from [`/libs`](./libs) are not in **your system PATH**, move the contents of `/libs` to the main folder
3. the file extension for ***Cltext*** is `.clt`
4. **(Optional)** install the Cltext VS Code extension
    * use `code --install-extension C:\Cltext\optionals\cltext-support-*.vsix` to install

## Quick Start:

Create a file called `hello.clt`:

```clt
<bold>Hello, world!</bold>
```

Run it:

```bash
cltext hello.clt
```

Output:

| |
|---|
| **Hello, world!** |

## Usage:

- any text inside a `.clt` file gets rendered
- to open a `.clt` file use 
```bash
cltext filename.clt
```
- or check info with
```bash
cltext [-v | --version] [extension | cltext]
cltext [-h | --help]
```

### Basic tags ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- to make a text **BOLD** use `<bold>your text</bold>`
- to make a text *italic* use `<tilt>your text</tilt>`
- to make text small use `<small>your text</small>`
- to make text big use `<big>your text</big>`
- to make text render in reverse use `<reverse>your text</reverse>`
    * example of reverse text: `!dlrow ,olleH`
- to color text use `<color c="#ffffff">your text</color>`

### Animated tags: ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- wavy text can be made using `<wave a=amount>your text</wave>`
- bouncy text can be made using `<bounce a=amount>your text</bounce>`
- text that shakes can be made using `<shake a=amount>your text</shake>`
- to make text glitchy use `<glitch>your text</glitch>`
- to make text spin use `<spin a=1>your text</spin>`

### Document tags: ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- to change the document background color use `<bgcolor c="#ff0000">`
- to change the document default text color from that point onward use `<txtcolor c="#000000">`
- to change window title use `<title name="your title">`

### Comments: ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- to add a comment use `<?== comment ==?>` or `<?==comment==?>`

### Miscellaneous: ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- to add a line use `<line>`
    - to make the line a color use `<line c="#000000">`
    - to make a line a certain length use `<line l=2>`
    - you can use both `<line l=2 c="#000000">`

# Made by:

- [pasztet211](https://github.com/pasztet211) ![pasztet](https://img.shields.io/badge/creator-orange?style=flat-square)
