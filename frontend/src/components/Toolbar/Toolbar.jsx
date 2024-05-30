import React from 'react';
import './Toolbar.css';
import ControlPanel from '../ControlPanel/ControlPanel';

function Toolbar( {handleCrop} ) {
  return (
    <div className="toolbar">
      <ControlPanel type="resize" />
      <ControlPanel type="brightness" />
      <ControlPanel type="contrast" />
      <ControlPanel type="blur" />
      <ControlPanel type="rotate" />
      <ControlPanel type="crop" handleCrop={handleCrop} />
      <ControlPanel type="download" />
      <ControlPanel type="reset" />
    </div>
  );
}

export default Toolbar;
