# Developer Log

## [11/27/25] - Initial Setup and Looking over the project
* **Work Done**: Initialized git repo, added started files (Cmake). Created 'developer_log.md'
* **Issues**: None....I'd hope so since I haven't done anything :)
* **Files Modified**: developer_log.md

## [11/28/2025] - Initial Work in tree.h
* **Work Done**: Initial Attempt at the structure and code of all the methods in tree.h
* **Issues**: None detected, but Haven't fully integrated proper testing yet. Did have some funny errors for confusing inline and outline in adding in the methods....I was tired after work and was not thinking for a bit....
* **Files Modified**: developer_log.md, tree.h

## [11/29/2025 -> 11/30/2025] - More cleanup in tree.h, beginning work in main.cpp
* **Work Done**: Added the playGame() method to tree.h, and started working on the main.cpp file on what needed done
* **Issues**: Would randomly error out at certain times...Usually a syntax error, but once it might have been just from a wrong approach from an earlier day that I corrected for...hopefully. It is still erroring out, but it is 1am and I am not sure WHY it is erroring out...something about the connection to openAI not working? The port number in a bad range? Will investigate further after sleep...
* **Files Modified**: developer_log.md, tree.h, main.cpp

## [12/1/25] - Changing main.cpp slightly to verify tree functionality
* **Work Done**: Made a direct call to a premade story.txt file, to verify tree functionality
* **Issues**: Rewrote main.cpp to call a premade story.txt. It does work this way, but it no longer has OpenAI functionality. Will need to try and fix that somehow, but have no idea WHY....
* **Files Modified**: developer_log.md, main.cpp, premade story.txt

## [12/3/25-12/5/25] - Continued Debugging
* **Work Done**: Remade OpenAI call. Verified it sometimes works in some ways, and sometimes breaks
* **Issues**: Now I am really confused. Sometimes, it works fine, and other times not. When it does work, it is buggy (sometimes has 17 children, but never connects to anything), or it'll just fail, saying the curl command is wrong....I am at a bit of a lost now.
* **Files Modified**: developer_log.md, main.cpp, tree.h

## [12/9/25] - Finally Complete?
* **Work Done**: Remade OpenAI call. Added some debugging codes.
* **Issues**: I think it should work now? I re-editted a few things slightly in main.cpp and tree.h, adding some debugging lines in hopes of learning how to fix it, and now it works? I think it needed to be rebuilt, and it suddenly works now?
* **Files Modified**: developer_log.md, main.cpp, tree.h
* **Notes:**: Personal API Key can be provided if needed. It was done using gpt-4o-mini. Additionally, it can sometimes loop (1->2->4->1), and I cannot seem to break it. Might be a quirk in OpenAI I cannot quite solve?