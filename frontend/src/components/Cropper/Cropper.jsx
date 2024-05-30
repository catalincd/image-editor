import './Cropper.css';
import { useState, useRef, useEffect } from 'react';
import ReactCrop from 'react-image-crop';
import 'react-image-crop/dist/ReactCrop.css';
import { canvasPreview } from './cropImage';

export default function Cropper({ url, onCancelCrop }) {
  const imgRef = useRef(null);
  const [crop, setCrop] = useState(null);
  const [height, setHeight] = useState('');
  const [width, setWidth] = useState('');
  const [completedCrop, setCompletedCrop] = useState();
  const [imageSrc, setImageSrc] = useState(url);
  
  useEffect(() => {
    setImageSrc(url);
  }, [url]);


  const download = async () => {
    await canvasPreview(imgRef.current, completedCrop);
  };

  const onImageLoad = (e) => {
    setHeight(e?.currentTarget?.height);
    setWidth(e?.currentTarget?.width);
    setCompletedCrop({
      x: 0,
      y: 0,
      height: e?.currentTarget?.height,
      width: e?.currentTarget?.width,
      unit: 'px'
    });
  };

  return (
    <div className={'outerDiv'}>
      <div className="crop-container">
        <ReactCrop
          src={imageSrc}
          crop={crop}
          onChange={(_, percentCrop) => {
            setCrop(percentCrop);
          }}
          onComplete={(e) => {
            if (e?.height === 0 || e?.width === 0) {
              setCompletedCrop({
                x: 0,
                y: 0,
                height: height,
                width: width,
                unit: 'px'
              });
            } else {
              setCompletedCrop(e);
            }
          }}
        >
          <img
            ref={imgRef}
            crossorigin='anonymous'
            alt='Error'
            src={imageSrc}
            onLoad={onImageLoad}
          />
        </ReactCrop>
      </div>
      <div className={'controlsIcon'}>
        <button class="btn btn-primary" onClick={download}>Download</button>
        <button class="btn btn-primary" onClick={onCancelCrop}>Cancel Crop</button>
      </div>
    </div>
  );
}
