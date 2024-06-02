### Introduction

> Under development. Don't recommend using

The framework provides with easy API for building application on X11.

Features:
1. Routing
2. Handling events separately for each category: Button, Mouse, Keyboard
3. Styling separately
4. etc...

---

### Installing

Installing process is simple.

Downloading: 
```bash
git clone git@github.com:rahman-nero/x11-framework.git
```

Building:
```
make
```

---
### Running


To run this application:
```
./xwindow
```


---

### TODO

Features:
- [x] Ability to handle clicks on buttons
- [x] Ability to handle keyboard
- [x] Local state
- [ ] Global state so that different pages could mutate one global state 
- [ ] Ability to move between routes when clicked
- [ ] Ability to handle multiple events (keyboard, clicks)
- [ ] Ability to listen to Focus/MouseOver etc events
- [ ] Text output
  - [ ] Ability to change text color (Must create GC separately for each windows with text)
  - [ ] Ability to change fonts
  - [ ] Ability to change size etc
- [ ] Somehow implement scrolling
- [ ] Support UTF8 
- [ ] Implement
    - [ ] form input tag
    - [ ] image tag (icons as well)
    - [ ] video tag (gonna be hard)
    - [ ] audio tag (gonna be hard)

Must do:
- [ ] Implement Garbage collector
- [ ] Free all resource at the end
