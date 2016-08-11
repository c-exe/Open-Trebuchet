# Open Trebuchet Markup Language

Previous versions of *Trebuchet* have only offered plain text, however, the plan with Open Trebuchet is to offer far more.  This leaves a choice of three possibilities: a WYSIWYG Rich Text Editor which will be hard to make and harder to maintain, an EasyWorship-style Rich Text Editor that only shows the styles properly on the preview and probably won't work properly on Windows 10, or a simple markup language that may initially appear harder than it actually is.  For reasons of simplicity, maintainability, compatibility and actually getting this project off the ground, the third option seems to make the most sense.

The Open Trebuchet Markup Language will have tags demarkated by square brackets ("[" and "]").  Intro, Verse, Chorus, Bridge, Tag, Ending and Slide markers will each have tags, but so will adjustment to text display such as Bold, Italics and Colour.  Literal square brackets will be escaped by the square brackets themselves, i.e. "[[" and "[]" for "[" and "]" respectively.  Tags will not be closed XML-style but rather the fonts will revert to normal with a "Normal" tag (RTF-style).  All tags will be case insensitive!  In addition, slides - however they are marked - will be separated from each other by a double line break (as in EasyWorship) to distinguish them from simply a forced line break.  If a double line break is required within a slide, it can be done by putting a space (" ") on one of the lines.

It is worth noting that all tags are optional, though without the slide markers it may be difficult to know what parts of the song are for what purpose.

## Tags

The following is a list of the planned tags.

### Escapes

Tag | Meaning
--- | -------
`[[` | [
`[]` | ]

### Markers

Tag | Meaning
--- | -------
`[Intro]` | "*Intro*" marker
`[Intro 1]` | "*Intro 1*" marker (1 can be any number from 0 to 65535)
`[Ending]` | "*Ending*" marker
`[Ending 1]` | "*Ending 1*" marker (1 can be any number from 0 to 65535)
`[Outro]` | Alternative "*Ending*" marker
`[Outro 1]` | Alternative "*Ending 1*" marker (1 can be any number from 0 to 65535)
`[Verse]` | "*Verse*" marker
`[Verse 1]` | "*Verse 1*" marker (1 can be any number from 0 to 65535)
`[Chorus]` | "*Chorus*" marker
`[Chorus 1]` | "*Chorus 1*" marker (1 can be any number from 0 to 65535)
`[Bridge]` | "*Bridge*" marker
`[Bridge 1]` | "*Bridge 1*" marker (1 can be any number from 0 to 65535)
`[MiddleEight]` | Alternative "*Bridge*" marker
`[MiddleEight 1]` | Alternative "*Bridge 1*" marker (1 can be any number from 0 to 65535)
`[Tag]` | "*Tag*" marker
`[Tag 1]` | "*Tag 1*" marker (1 can be any number from 0 to 65535)
`[Slide]` | "*Slide*" marker
`[Slide 1]` | "*Slide 1*" marker (1 can be any number from 0 to 65535)

### Text Style

Tag | Meaning
--- | -------
`[=N]` | Revert all styles to Normal (this tag is assumed at the start of each song)
`[=B=1]` | Turn on Bold explicitly
`[=B=0]` | Turn off Bold explicitly
`[=B=N]` | Revert Bolding to the normal behaviour
`[=I=1]` | Turn on Italics explicitly
`[=I=0]` | Turn off Italics explicitly
`[=I=N]` | Revert Italicising to the normal behaviour
`[=U=1]` | Turn on Underline explicitly
`[=U=0]` | Turn off Underline explicitly
`[=U=N]` | Revert Underlining to the normal behaviour
`[=SH=1]` | Turn on Shadow explicitly
`[=SH=0]` | Turn off Shadow explicitly
`[=SH=N]` | Revert Shadowing to the normal behaviour
`[=OB=1]` | Turn on Black Outline explicitly
`[=OB=0]` | Turn off Black Outline explicitly
`[=OB=N]` | Revert Black Outlining to the normal behaviour
`[=OW=1]` | Turn on White Outline explicitly
`[=OW=0]` | Turn off White Outline explicitly
`[=OW=N]` | Revert White Outlining to the normal behaviour
`[=OI=1]` | Turn on Inverse Outline explicitly (where white text has a black border and vice versa)
`[=OI=0]` | Turn off Inverse Outline explicitly
`[=OI=N]` | Revert Inverse Outlining to the normal behaviour
`[=C=Red]` | Change colour to red
`[=C=Yellow]` | Change colour to yellow
`[=C=Green]` | Change colour to green
`[=C=Blue]` | Change colour to blue
`[=C=Cyan]` | Change colour to cyan
`[=C=Magenta]` | Change colour to magenta
`[=C=Black]` | Change colour to black
`[=C=White]` | Change colour to white
`[=C=Grey]` | Change colour to grey
`[=C=Gray]` | Change colour to grey
`[=C=Silver]` | Change colour to silver
`[=C=#A0DF00]` | Change colour to hexadecimal code A0DF00
`[=C=N]` | Change colour to normal colour
`[=S=15]` | Change text size to 15pt explicitly (not recommended)
`[=S=F]` | Fit the text size to the screen explicitly (the default for Normal)
`[=S=-1]` | Change text size to 1pt below the current "Fit to Screen" size
`[=S=+1]` | Change text size to 1pt above the current "Fit to Screen" size
`[=S=N]` | Change text size to normal size setting
