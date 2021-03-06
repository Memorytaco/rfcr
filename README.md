# RFC Reader

This is a simple rfc document reader with offline experience.

It's now not perfect and WIP.

# Install Process

## build executable
``` bash
git clone <repository>
cd <repository>
git submodule init
git submodule update
make
```

you will got an executable `rfcr` in the root directory.

## prepare environment

### sync the rfc repository

using `./install` to install config files and sync rfc repository.

> setting `RSYNC_PROXY` with format "host:port" if you want to use
> proxy. if you use privoxy then `export RSYNC_PROXY=127.0.0.1:8118`
> will do the magic.

now you are free to do your job when waiting this process being completed.

### set up configuration

configuration is in simple yaml format. and the file is denoted by environment variable.

``` bash
export RFCONF="$HOME/.rfc/conf.yml"
```

a template file is located in "etc" directory. copy it as "$HOME/.rfc/conf.yml" and edit it.

> following the comments.

you can choose some of your favorite *mono* fonts and configure it in your config file in **font** field.

after all the settings, type `rfcr conf all` to show all config values.

the output of `rfcr conf all`:
```txt
font => RobotoMono-VariableFont_wght.ttf;
fontdir => $YOURHOME/.local/share/fonts/;
shaderdir => $YOURHOME/.rfc/shaders/;
repo => $YOURHOME/.rfc/repo/;
```

## enjoy!

- rfcr show <number> : display the document, use 'f' and 'b' to navigate between pages. and 'd', 'u' scroll. mouse scroll will work.
- rfcr query <regex> [number] : using regex to search rfc title. add an optional number to limit output entries.
- rfcr conf <key> : if key is all, show all values or just show the specific value.

# Any Issue or pull request is Welcome!
