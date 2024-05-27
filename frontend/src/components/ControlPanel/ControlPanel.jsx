import React, { useState } from 'react';
import './ControlPanel.css';
import { processImage } from '../../api';

function ControlPanel({ type, handleCrop }) {
  const [width, setWidth] = useState(0);
  const [height, setHeight] = useState(0);
  const [brightness, setBrightness] = useState(100);
  const [blur, setBlur] = useState(0);
  const [rotation, setRotation] = useState(0);

  const applySettings = async () => {
    const payload = {
      target: 'target_value',
      resize: width > 0 || height > 0,
      size: { width, height },
      recontrast: false,
      contrast: 0.0,
      rebright: true,
      brightness: brightness / 100,
    };

    // Handling the response from the API
    try {
      const result = await processImage(payload);
      console.log("Image processed successfully:", result);
    } catch (error) {
      console.error("Error processing image:", error);
    }
  };

  const renderControl = () => {
    switch (type) {
      case 'resize':
        return (
          <div className="resize-controls">
            <label>Width:
              <input type="number" value={width} onChange={e => setWidth(e.target.value)} />
            </label>
            <label>Height:
              <input type="number" value={height} onChange={e => setHeight(e.target.value)} />
            </label>
            <button onClick={() => console.log(`Resize to: ${width}x${height}`)}>Apply</button>
            {/* <button onClick={applySettings}>Apply</button> */}
          </div>
        );
      case 'brightness':
        return (
          <div className="brightness-controls">
            <label>Brightness:
              <input type="range" min="0" max="200" value={brightness} onChange={e => setBrightness(e.target.value)} />
            </label>
            <button onClick={() => console.log(`Set brightness to: ${brightness}%`)}>Apply</button>
            {/* <button onClick={applySettings}>Apply</button> */}
          </div>
        );
      case 'blur':
        return (
          <div className="blur-controls">
            <label>Blur:
              <input type="range" min="0" max="10" value={blur} onChange={e => setBlur(e.target.value)} />
            </label>
            <button onClick={() => console.log(`Set blur to: ${blur}px`)}>Apply</button>
            {/* <button onClick={applySettings}>Apply</button> */}
          </div>
        );
      case 'rotate':
        return (
          <div className="rotate-controls">
            <label>Rotate:
              <input type="number" value={rotation} onChange={e => setRotation(e.target.value)} />
            </label>
            <button onClick={() => console.log(`Rotate to: ${rotation} degrees`)}>Apply</button>
            {/* <button onClick={applySettings}>Apply</button> */}
          </div>
        );
      case 'crop':
        return (
          <div className="crop-controls">
            <button onClick={handleCrop}>Crop</button>
          </div>
        );
      default:
        return <p>No control selected</p>;
    }
  };

  return (
    <div className={`control-panel ${type}`}>
      <h3>{type.charAt(0).toUpperCase() + type.slice(1)}</h3>
      {renderControl()}
    </div>
  );
}

export default ControlPanel;
