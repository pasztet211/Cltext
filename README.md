![C23](https://img.shields.io/badge/C-C23-blue?style=flat-square)
![SDL2](https://img.shields.io/badge/SDL2-required-green?style=flat-square)
![SDL2_ttf](https://img.shields.io/badge/SDL2__ttf-required-green?style=flat-square)

![version](https://img.shields.io/badge/version-v0.2.3-orange?style=flat-square)
![extension](https://img.shields.io/badge/extension-v0.2.42-orange?style=flat-square)

# Cltext

markup language

## demo:

https://github.com/user-attachments/assets/0478ef44-1c99-48fd-8d0c-ef54d2559c34

<details>
<summary>Code</summary>

```clt
<txtcolor c="#000000">
<bgcolor c="#ff0000">
<title name="example">
<bold><line l=3 c="#ffffff"></bold>
<reverse>there is this</reverse> <bold><wave a=2>thingy</wave></bold> called a <bold>particle</bold>
<shake a=1>it</shake> is <spin a=1>very</spin> <tilt><small>small.</small></tilt>
<?==
 comment 
==?>
<bold><glitch>it</glitch> is <wave a=0.5>part <big>of</big> an <bounce a=0.5><color c="#0000ff"><outline t=2 c="#ffffff">atom</outline></color></bounce></wave>.</bold>
<bold><line c="#ffffff" l=3></bold>
<?== other comment ==?>
<bold><randomcolor>text</randomcolor></bold> (i gave up)

















<bold>secret text</bold> (you can scroll)
```
</details>

## Installation and file extension:

**Option 1: Clone the repo**

1. Clone the repo to `C:\Cltext`
   * If the SDL2 DLLs (e.g. `SDL2_ttf.dll`) from [`/libs`](./libs) are not in **your system PATH**, move the contents of `/libs` to the main folder.

2. Add `C:\Cltext` to **PATH**[^second]

**Option 2: Quick download**

1. Download [`Cltext-v0.2.3-windows.zip`](./Cltext-v0.2.3-windows.zip)
   * The ZIP already includes Cltext and the required SDL2 DLLs.
  
<img width="957" height="117" alt="download help" src="https://github.com/user-attachments/assets/3eb41568-0b2e-4a8f-9ab2-f6c62965e10a" />

2. Unzip it and put the contents into `C:\Cltext`

3. Add `C:\Cltext` to **PATH**[^second]

The file extension for **Cltext** is `.clt`

**(Optional)** Install the Cltext VS Code extension
   * Use:
     ```bash
     code --install-extension C:\Cltext\optionals\cltext-support-*.vsix
     ```

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

- to add a outline use `<outline>your text</outline>`
    - to make the outline a color use `<outline c="#000000">your text</outline>`
    - to make a outline a certain thickness use `<outline t=2>your text</outline>`
    - you can use both `<outline t=2 c="#000000">your text</outline>`

- to make each letter of text a random color use `<randomcolor>your text</randomcolor>`[^first]

## More Info:

- if your document does not fit on the screen you can scroll to see more

# Made by:

- [pasztet211](https://github.com/pasztet211) ![pasztet](https://img.shields.io/badge/creator-orange?style=flat-square)

[^first]: to specify a random **VIBRANT** color
[^second]:here is a guide 🔗[how to add to path](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/)
