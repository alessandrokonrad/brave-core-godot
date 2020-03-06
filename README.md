[![Build Status](https://travis-ci.org/brave/brave-core.svg?branch=master)](https://travis-ci.org/brave/brave-core)

# Brave Core

Brave Core is a set of changes, APIs, and scripts used for customizing Chromium to make Brave.

Follow [@brave](https://twitter.com/brave) on Twitter for important
announcements.

## Godot Implementation

### Explanation

This is just a fork of the brave-core repository. To run a full development environment please refer to [Brave Wiki](https://github.com/brave/brave-browser/wiki).
Before you call `npm run init` go to `src` (if not there just create a folder) and add this fork under the folder `src/brave`. Then continue with the build instructions.
All file changes are made in this fork in order to implement the godot scene, even if it's necessary to edit the chromium source.
Read more about it [here](https://github.com/brave/brave-browser/wiki/Patching-Chromium)

### Files I modified

brave-browser/src/content/public/browser/url_data_source.cc (patched with: ./patches/content-public-browser-url_data_source.cc.patch)  
./components/webpack/webpack.config.js  
./components/brave_new_tab_ui/brave_new_tab.tsx

### New Files I added

./components/game-godot/game.tsx  
./components/game-godot/index.js
./components/game-godot/index.pck  
./components/game-godot/index.wasm  
./components/game-godot/index.css

### What to do

I might change the CSP, so that not all pages have the 'unsafe-eval' directive. url_data_manager_backend.cc could be a file I need to modify for it, but still not sure how.

### Steps done

- creating a sample godot scene
- compiling it to WebGL
- modifing files, so that it's easier to implement
- creating a folder game-godot inside ./components/brave_new_tab_ui/ and copy game files inside
- creating a game.tsx file inside game folder, which acts as component and wrapps everything together (component is freely movable to any page)
- allowing the Content-Security-Policy header 'unsafe-eval' in the file url_data_source.cc, so that WebAssembly code can be executed and the scene can be loaded
- adding a file-loader in the webpack.config.js, so that game files are available as resources in brave (e.g. brave://newtab/index.js)
- editing brave_new_tab.tsx and insert the component, so that scene is shown at brave://newtab (new tab page)

## Resources

- [Issues](https://github.com/brave/brave-browser/issues)
- [Releases](https://github.com/brave/brave-browser/releases)
- [Documentation wiki](https://github.com/brave/brave-browser/wiki)

## Community

You can ask questions and interact with the community in the following
locations:

- [The Brave Community](https://community.brave.com/)
- [`community`](https://bravesoftware.slack.com) channel on Brave Software's Slack
- [`developers-brave`](https://discord.gg/k57tYrS) channel in our Brave Discord server
