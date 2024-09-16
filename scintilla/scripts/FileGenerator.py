#!/usr/bin/env python3
# FileGenerator.py - implemented 2013 by Neil Hodgson neilh@scintilla.org
# Released to the public domain.

# Generate or regenerate source files based on comments in those files.
# May be modified in-place or a template may be generated into a complete file.
# Requires Python 2.7 or later
# The files are copied to a string apart from sections between a
# ++Autogenerated comment and a --Autogenerated comment which is
# generated by the CopyWithInsertion function. After the whole string is
# instantiated, it is compared with the target file and if different the file
# is rewritten.

from __future__ import with_statement

import codecs, os, re, string, sys

lineEnd = "\r\n" if sys.platform == "win32" else "\n"

def UpdateFile(filename, updated):
    """ If the file contents are different to updated then copy updated into the
    file else leave alone so Mercurial and make don't treat it as modified. """
    newOrChanged = "Changed"
    try:
        with codecs.open(filename, "r", "utf-8") as infile:
            original = infile.read()
        if updated == original:
            # Same as before so don't write
            return
        os.unlink(filename)
    except IOError: # File is not there yet
        newOrChanged = "New"
    with codecs.open(filename, "w", "utf-8") as outfile:
        outfile.write(updated)
    print("%s:0: %s" % (filename, newOrChanged))

# Automatically generated sections contain start and end comments,
# a definition line and the results.
# The results are replaced by regenerating based on the definition line.
# The definition line is a comment prefix followed by "**".
# If there is a digit after the ** then this indicates which list to use
# and the digit and next character are not part of the definition
# Backslash is used as an escape within the definition line.
# The part between \( and \) is repeated for each item in the list.
# \* is replaced by each list item. \t, and \n are tab and newline.
# If there is no definition line than the first list is copied verbatim.
# If retainDefs then the comments controlling generation are copied.
def CopyWithInsertion(input, commentPrefix, retainDefs, lists):
    copying = 1
    generated = False
    listid = 0
    output = []
    for line in input.splitlines(0):
        isStartGenerated = line.lstrip().startswith(commentPrefix + "++Autogenerated")
        if copying and not isStartGenerated:
            output.append(line)
        if isStartGenerated:
            if retainDefs:
                output.append(line)
            copying = 0
            generated = False
        elif not copying and not generated:
            # Generating
            if line.startswith(commentPrefix + "**"):
                # Pattern to transform input data
                if retainDefs:
                    output.append(line)
                definition = line[len(commentPrefix + "**"):]
                if (commentPrefix == "<!--") and (" -->" in definition):
                    definition = definition.replace(" -->", "")
                listid = 0
                if definition[0] in string.digits:
                    listid = int(definition[:1])
                    definition = definition[2:]
                # Hide double slashes as a control character
                definition = definition.replace("\\\\", "\001")
                # Do some normal C style transforms
                definition = definition.replace("\\n", "\n")
                definition = definition.replace("\\t", "\t")
                # Get the doubled backslashes back as single backslashes
                definition = definition.replace("\001", "\\")
                startRepeat = definition.find("\\(")
                endRepeat = definition.find("\\)")
                intro = definition[:startRepeat]
                out = ""
                if intro.endswith("\n"):
                    pos = 0
                else:
                    pos = len(intro)
                out += intro
                middle = definition[startRepeat+2:endRepeat]
                for i in lists[listid]:
                    item = middle.replace("\\*", i)
                    if pos and (pos + len(item) >= 80):
                        out += "\\\n"
                        pos = 0
                    out += item
                    pos += len(item)
                    if item.endswith("\n"):
                        pos = 0
                outro = definition[endRepeat+2:]
                out += outro
                out = out.replace("\n", lineEnd) # correct EOLs in generated content
                output.append(out)
            else:
                # Simple form with no rule to transform input
                output.extend(lists[0])
            generated = True
        if line.lstrip().startswith(commentPrefix + "--Autogenerated") or \
            line.lstrip().startswith(commentPrefix + "~~Autogenerated"):
            copying = 1
            if retainDefs:
                output.append(line)
    output = [line.rstrip(" \t") for line in output] # trim trailing whitespace
    return lineEnd.join(output) + lineEnd

def GenerateFile(inpath, outpath, commentPrefix, retainDefs, *lists):
    """Generate 'outpath' from 'inpath'.
    """

    try:
        with codecs.open(inpath, "r", "UTF-8") as infile:
            original = infile.read()
        updated = CopyWithInsertion(original, commentPrefix,
            retainDefs, lists)
        UpdateFile(outpath, updated)
    except IOError:
        print("Can not open %s" % inpath)

def Generate(inpath, outpath, commentPrefix, *lists):
    """Generate 'outpath' from 'inpath'.
    """
    GenerateFile(inpath, outpath, commentPrefix, inpath == outpath, *lists)

def Regenerate(filename, commentPrefix, *lists):
    """Regenerate the given file.
    """
    Generate(filename, filename, commentPrefix, *lists)

def UpdateLineInPlistFile(path, key, value):
    """Replace a single string value preceded by 'key' in an XML plist file.
    """
    lines = []
    keyCurrent = ""
    with codecs.open(path, "rb", "utf-8") as f:
        for line in f.readlines():
            ls = line.strip()
            if ls.startswith("<key>"):
                keyCurrent = ls.replace("<key>", "").replace("</key>", "")
            elif ls.startswith("<string>"):
                if keyCurrent == key:
                    start, tag, rest = line.partition("<string>")
                    _val, etag, end = rest.partition("</string>")
                    line = start + tag + value + etag + end
            lines.append(line)
    contents = "".join(lines)
    UpdateFile(path, contents)

def UpdateLineInFile(path, linePrefix, lineReplace):
    lines = []
    updated = False
    with codecs.open(path, "r", "utf-8") as f:
        for line in f.readlines():
            line = line.rstrip()
            if not updated and line.startswith(linePrefix):
                lines.append(lineReplace)
                updated = True
            else:
                lines.append(line)
    if not updated:
        print(f"{path}:0: Can't find '{linePrefix}'")
    contents = lineEnd.join(lines) + lineEnd
    UpdateFile(path, contents)

def UpdateFileFromLines(path, lines, lineEndToUse):
    """Join the lines with the lineEndToUse then update file if the result is different.
    """
    contents = lineEndToUse.join(lines) + lineEndToUse
    UpdateFile(path, contents)

def ReplaceREInFile(path, match, replace, count=1):
    with codecs.open(path, "r", "utf-8") as f:
        contents = f.read()
    contents = re.sub(match, replace, contents, count)
    UpdateFile(path, contents)
