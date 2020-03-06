import * as React from "react";

/* Godot WebGL files, loaded as resources in brave */
require("./index.pck");
require("./index.js");
require("./index.wasm");
require("./index.css");

const Game = () => {
  const appendScript = (src: string) => {
    const script = document.createElement("script");
    script.type = "text/javascript";
    script.src = src;
    document.body.appendChild(script);
  };

  React.useEffect(() => {
    /* order of script appending is important */
    appendScript("index.js");
    //appendScript("engine.js");
  });

  return (
    <React.Fragment>
      <canvas id="canvas">
        HTML5 canvas appears to be unsupported in the current browser.
        <br />
        Please try updating or use a different browser.
      </canvas>
      <div id="status">
        <div
          id="status-progress"
          style={{ display: "none" }}
          onContextMenu={(event: any) => event.preventDefault()}
        >
          <div id="status-progress-inner"></div>
        </div>
        <div
          id="status-indeterminate"
          style={{ display: "none" }}
          onContextMenu={(event: any) => event.preventDefault()}
        >
          <div></div>
          <div></div>
          <div></div>
          <div></div>
          <div></div>
          <div></div>
          <div></div>
          <div></div>
        </div>
        <div
          id="status-notice"
          className="godot"
          style={{ display: "none" }}
        ></div>
      </div>
    </React.Fragment>
  );
};

export default Game;
