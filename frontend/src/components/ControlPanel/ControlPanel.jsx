import React, { useContext, useEffect, useState } from 'react';
import './ControlPanel.css';

import { processImage } from '../../api'

import { ImageContext } from "../../ImageContext"

function ControlPanel({ type, handleCrop }) {

  const { image, setImage } = useContext(ImageContext);

  const [width, setWidth] = useState(0);
  const [height, setHeight] = useState(0);
  const [brightness, setBrightness] = useState(100);
  const [contrast, setContrast] = useState(2);
  const [blur, setBlur] = useState(0);
  const [rotation, setRotation] = useState(0);

  /*
  useEffect(() => {
    processImage(image, setImage, { angle: rotation })
  }, [rotation])

  useEffect(() => {
    processImage(image, setImage, { blur })
  }, [blur])

  useEffect(() => {
    processImage(image, setImage, { contrast })
  }, [contrast])

  useEffect(() => {
    processImage(image, setImage, { brightness })
  }, [brightness])

  useEffect(() => {
    processImage(image, setImage, { size: { width, height } })
  }, [width, height])
  */

  const renderControl = () => {
    switch (type) {
      case 'resize':
        return (
          <div className="resize-controls">
            <label>Width:
              <input type="number" value={width} onChange={e => setWidth(parseInt(e.target.value))} />
            </label>
            <label>Height:
              <input type="number" value={height} onChange={e => setHeight(parseInt(e.target.value))} />
            </label>
            <button onClick={() => processImage(image, setImage, { size: { width, height } })}>Apply</button>
            {/* <button onClick={applySettings}>Apply</button> */}
          </div>
        );
      case 'brightness':
        return (
          <div className="brightness-controls">
            <label>Brightness:
              <input type="range" min="0" max="200" value={brightness} onChange={e => setBrightness(parseInt(e.target.value))} />
            </label>
            <button onClick={() => processImage(image, setImage, { brightness })}>Apply</button>
            {/* <button onClick={applySettings}>Apply</button> */}
          </div>
        );
      case 'contrast':
        return (
          <div className="brightness-controls">
            <label>Contrast:
              <input type="range" min="0" max="200" value={contrast} onChange={e => setContrast(parseInt(e.target.value))} />
            </label>
            <button onClick={() => processImage(image, setImage, { contrast })}>Apply</button>
            {/* <button onClick={applySettings}>Apply</button> */}
          </div>
        );
      case 'blur':
        return (
          <div className="blur-controls">
            <label>Blur:
              <input type="range" min="0" max="10" value={blur} onChange={e => setBlur(parseFloat(e.target.value))} />
            </label>
            <button onClick={() => processImage(image, setImage, { blur })}>Apply</button>
            {/* <button onClick={applySettings}>Apply</button> */}
          </div>
        );
      case 'rotate':
        return (
          <div className="rotate-controls">
            <label>Rotate:
              <input type="number" value={rotation} onChange={e => setRotation(parseInt(e.target.value))} />
            </label>
            <button onClick={() => processImage(image, setImage, { angle: rotation })}>Apply</button>
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
