import React, { useState } from 'react';
import 'bootstrap/dist/css/bootstrap.min.css';
import './App.css';
import Toolbar from './components/Toolbar/Toolbar';
import ImageEditor from './components/ImageEditor/ImageEditor';

function App() {
  const [image, setImage] = useState(null);
  const [darkMode, setDarkMode] = useState(false);

  const toggleDarkMode = () => {
    setDarkMode(!darkMode);
    document.body.classList.toggle('dark-mode');
  };

  const handleImageChange = (event) => {
    const file = event.target.files[0] || event.dataTransfer.files[0];
    processFile(file);
  };

  const resetAndGoHome = () => setImage(null);

  const processFile = (file) => {
    const reader = new FileReader();
    reader.onloadend = () => {
      setImage(reader.result);
    };
    reader.readAsDataURL(file);
  };

  const handleDragOver = (event) => {
    event.preventDefault();
  };

  const handleDrop = (event) => {
    event.preventDefault();
    const file = event.dataTransfer.files[0];
    processFile(file);
  };

  return (
    <div className={`App ${darkMode ? 'bg-dark text-white' : 'bg-light'}`}>
      <div className="top-toolbar d-flex justify-content-between align-items-center px-3 py-2">
        <img src="/logo192.png" alt="Logo" className="App-logo" onClick={resetAndGoHome} />
        <button onClick={toggleDarkMode} className={`btn btn-${darkMode ? 'light' : 'secondary'} ml-auto`}>
          {darkMode ? 'Light Mode' : 'Dark Mode'}
        </button>
      </div>
      {!image ? (
        <main className="container mt-5">
            <div className="upload-section text-center p-4 mt-5 border rounded mx-auto" style={{ maxWidth: '500px' }} onDragOver={handleDragOver} onDrop={handleDrop}>
            <h1>Welcome to Image Editor</h1>
            <input type="file" className="form-control mt-3" onChange={handleImageChange} />
            </div>
        </main>
      ) : (
        <>
          <Toolbar />
          <ImageEditor image={image} />
        </>
      )}
    </div>
  );
}

export default App;
