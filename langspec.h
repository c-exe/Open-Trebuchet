#ifndef __LANGSPEC_H__
#define __LANGSPEC_H__

/*
 * Language Specification File.
 *
 * Use this file to specify the language for which you want to compile Open Trebuchet.
 *
 * Languages:
 * For Language         | Use Value
 * ---------------------+--------------------
 * English (UK)         | _LANG_ENGLISH
 * French               | _LANG_FRANCAIS
 * Welsh                | _LANG_CYMRAIG
 * ---------------------+--------------------
 *
 * The following sections are for defining the chosen language for various features.  Use
 * the values from the above table.
 */
 
/*
 * Menus
 * Use the following statement to define the language to use for Open Trebuchet's menu 
 * system.
 */

#define MENULANG _LANG_ENGLISH

/*
 * Dialogue Boxes
 * Use the following statement to define the language to use for Open Trebuchet's various
 * dialogue boxes.
 */

#define DIALOGUELANG _LANG_ENGLISH 

/*
 * Program Information
 * Use the following statement to define the language to use for Open Trebuchet's program
 * information.
 */

#define INFOLANG _LANG_ENGLISH

/*
 * Error Messages
 * Use the following statement to define the language to use for Open Trebuchet's error
 * messages.
 */

#define ERRORLANG _LANG_ENGLISH

/* -------------------------------------------------------------------------------- */

#endif
