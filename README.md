![C23](https://img.shields.io/badge/C-C23-blue?style=flat-square)
![SDL2](https://img.shields.io/badge/SDL2-required-green?style=flat-square)
![SDL2_ttf](https://img.shields.io/badge/SDL2__ttf-required-green?style=flat-square)

![version](https://img.shields.io/badge/version-v0.1.4-orange?style=flat-square)

# Cltext

markup language

## Installation and file extension:

1. clone the repo to `C:\Cltext`
2. add `C:\Cltext` to **PATH**
    - if the SDL2 DLLs [egg. SDL2_ttf.dll] from `/libs` are not in **PATH**, move the contents of `/libs` to the main folder
3. the file extension for ***Cltext*** is `.clt`
4. **(Optional)** install the Cltext VS Code extension
    - use `code --install-extension C:\Cltext\optionals\cltext-support-0.0.1.vsix` to install

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
cltext [-v | --version]
```

### Basic tags ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- to make a text **BOLD** use `<bold>your text</bold>`
- to make a text *italic* use `<tilt>your text</tilt>`
- to make text small use `<small>your text</small>`
- to make text big use `<big>your text</big>`

### Animated tags: ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- wavy text can be made using `<wave a=amount>your text</wave>`
- bouncy text can be made using `<bounce a=amount>your text</bounce>`
- text that shakes can be made using `<shake a=amount>your text</shake>`
- to color text use `<color c="#ffffff">your text</color>`

### Document tags: ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- changing the document bg color can be done using `<bgcolor c="#ff0000">`
- to change the document default text color from that point onward use `<txtcolor c="#000000">`

### Comments: ![works](https://img.shields.io/badge/status-stable-green?style=flat-square)

- to add a comment use `<?== comment ==?>`

# Made by:

- [pasztet211](https://github.com/pasztet211) ![pasztet](https://img.shields.io/badge/creator-orange?style=flat-square)