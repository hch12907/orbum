# Contributing to orbum
First things first: Thank you for your interest in contributing to orbum!

## Code style guidelines
* Struct naming: Usually `CamelCase`, but `CamelCase_AdditionalInfo` is acceptable. (examples: `VifUnit` and `GifRegister_Ctrl`)
* Function/method naming: `snake_case()`
* Variable/struct members naming: `snake_case` or `snake_case1` or `snake_case_1`.
* Braces placement: Curly braces should appear on the newline.
* Indentation: 4-width spaces.
* Comments: `/// something` or `// something`; mostly similar to the Rustaceans' usage (not strictly enforced, though...)

Note:
* Put state-tracking variables in Resources (either the unit's Ctrl register or the unit's struct). Controllers should strictly contain logical code only.
* (Usually you should) Use classes instead of namespaces
* When in doubt, run clang-format on your files.
```bash
    # run this command in the top directory! it is where .clang-format is located.
    clang-format -i -style=file <path-to-your-files>
```

## Issue/PR templates
* Um... I don't think we need it now. Maybe when the emulator starts booting games.

## Code of conduct
* Uhhhh... just don't spam or annoy people or something, be a nice person. That's it.
