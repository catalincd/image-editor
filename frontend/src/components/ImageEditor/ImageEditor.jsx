import React from 'react';
import './ImageEditor.css';

function ImageEditor({ image }) {
    return (
      <div className="image-editor">
        <img src={image} alt="Editable" style={{ maxWidth: '100%', maxHeight: '500px' }} />
      </div>
    );
  }

export default ImageEditor;
