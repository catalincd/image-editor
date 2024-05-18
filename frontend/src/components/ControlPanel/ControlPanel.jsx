import React, { useState } from 'react';
import './ControlPanel.css';

function ControlPanel({ type }) {
  const [width, setWidth] = useState('');
  const [height, setHeight] = useState('');
  const [brightness, setBrightness] = useState(100);
  const [blur, setBlur] = useState(0);
  const [rotation, setRotation] = useState(0);

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
          </div>
        );
      case 'brightness':
        return (
          <div className="brightness-controls">
            <label>Brightness:
              <input type="range" min="0" max="200" value={brightness} onChange={e => setBrightness(e.target.value)} />
            </label>
            <button onClick={() => console.log(`Set brightness to: ${brightness}%`)}>Apply</button>
          </div>
        );
      case 'blur':
        return (
          <div className="blur-controls">
            <label>Blur:
              <input type="range" min="0" max="10" value={blur} onChange={e => setBlur(e.target.value)} />
            </label>
            <button onClick={() => console.log(`Set blur to: ${blur}px`)}>Apply</button>
          </div>
        );
      case 'rotate':
        return (
          <div className="rotate-controls">
            <label>Rotate:
              <input type="number" value={rotation} onChange={e => setRotation(e.target.value)} />
            </label>
            <button onClick={() => console.log(`Rotate to: ${rotation} degrees`)}>Apply</button>
          </div>
        );
      case 'crop':
        return (
          <div className="crop-controls">
            <button onClick={() => console.log(`Crop image`)}>Crop</button>
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
