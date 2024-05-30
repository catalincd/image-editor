var targetOld = null;


export const resetTarget = () => targetOld = null;

export const processImage = async (image, setImage, configs) => {
  try {
    console.log(configs)

    var target = "x." + image.substr(11, 4)
    if (target[target.length - 1] === ';')
      target = target.substring(0, target.length - 1);

    targetOld = targetOld || target

    const payload = {
      target: targetOld,

      resize: configs.size ? true : false,
      size: configs.size || { width: -1, height: -1 },

      recontrast: configs.contrast ? true : false,
      contrast: configs.contrast || 1.0,

      rebright: configs.brightness ? true : false,
      brightness: configs.brightness || 50,

      blur: configs.blur ? true : false,
      kernel: configs.blur || 50,

      rotate: configs.angle ? true : false,
      angle: configs.angle || 0.0,

      crop: configs.cropData ? true : false,
      cropData: configs.cropData || { x: 0.0, y: 0.0, width: 0.0, height: 0.0 },

      data: image.substr(image.indexOf(',') + 1)
    }

    console.log(payload)

    const requestOptions = {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'X-Requested-With': 'XMLHttpRequest',
        'Access-Control-Allow-Origin': '*',
        'User-Agent': 'COX'
      },
//      mode: 'no-cors',
      body: JSON.stringify(payload)
    };

    fetch(`/send`, requestOptions)
    .then(response => response.blob())
    .then(blob => {
      const reader = new FileReader();
      reader.onloadend = () => {
        setImage(reader.result);
      };
      reader.readAsDataURL(blob);
    })
  

  } catch (error) {
    console.error("Error processing image:", error);
  }
};
