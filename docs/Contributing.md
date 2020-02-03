# Contributing Guide
This guide targets new members looking for a guide on contributing to the
project. This guide will focus on the overall picture of effective coding and
contributions. It is not intended to be a technical guide.

## Initial Notes
Contributing to open source presents a unique set of challenges, especially when
dealing with large projects. Our project is approximately 50-80k lines of code,
and is still very small by some open source standards.

This should not be discouraging, but rather informative. Most initial
contributions will be small; it takes time to learn the project structure and any
languages and skills each particular project requires. Contributions of any size
will be greatly appreciated, and you should have realistic expectations of what
a first contribution might be (if you dont believe me, you can look at my first
contribution [here](https://github.com/RoboJackets/robocup-software/commit/6ce98fc0f8d88b6d145700779e126c0f1b99bb92#diff-71a3477f37bd5b20744e292eda2e3fbc). It's two lines of 'code'). This guide will help you learn to navigate our code base, and
work toward an initial contribution.

## Pull Request Requirements
With large projects, organization and structure can break down fairly quickly
resulting in some bad spaghetti code. We want to avoid this as much as
possible. For this reason, every pull request will be reviewed by a more senior
member of the team, currently Justin Buchanan. If your request isn't accepted
right away, don't take it personally. Often your code may work fine, but there
are things you can add or refine. This helps keep our repository clean and will
give you valuable experience participating in a code review process.

### Continuous Integration
Continuous Integration (CI) is a tool to help auto-detect problems before they
are merged into the main respository and have a chance to cause problems. Every
time you submit a pull request, the CI tool is run and it will assign a passing
or failing mark to the request. If the CI fails, you will need to fix the error
in your code before the code review. If the reason for the failure isn't
obvious or it's a problem in the CI check itself, seek some help.

### Content
It's important that the content of a pull request be kept clean and small. Pull
requests should be less than 1-2k lines of code. The code changes should
reflect one and only one topic (e.g do not include two bug fixes in one pull
request). Content should generally be kept to code and documentation, binary
content, such as images, may be uploaded elsewhere.

### Documentation
Code should be documented thoroughly. Generally speaking, you won't be here for
more than 4(ish) years. Many students will come behind you and will need to use
the code you've written.

Each class or file you create should be documented as to what it contains and
what purpose it serves.

Each function should have documentation containing it's purpose, what
parameters is takes, and what values it returns. Error handling should be
described as well: what errors will it produce, and what assumptions it makes
regarding the validation the caller performs ahead of time. If relevant, state
if the function may block for extended periods of time. If applicable, state if
the function is reentrant or accquires and releases locks.

If the overall set of code is complex and new, consider editing or adding to
the wiki.

C/C++/Python is documented using doxygen. You can view the guide for writing doxygen
comments and documentation [here](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html).

Text file types that are not supported by doxygen should still be documented
using what ever commenting style that format supports.

### Style and Formatting
In order to keep the code more readable, code should be formatted and styled
uniformly. This would be difficult to coordinate across multiple users, so we
have a program that automatically restyles the code for you. If you submit a
pull request before restyling the code, it will likely fail the CI check. You
can auto-format the code by running `make pretty`. If you have a lot of code,
you may have to run this a few times. You can check if the style is passing
by running `make checkstyle`. If there are no errors, then you are good to go.
