let contrastVal = document.getElementById("contrastVal");
const contrastSlider = document.getElementById("contrastSlider");
let brightnessVal = document.getElementById("brightnessVal");
const brightnessSlider = document.getElementById("brightnessSlider");
const resetButton = document.getElementById("reset");
const saveButton = document.getElementById("save-button");
const backgroundUpload = document.getElementById("background-upload");
const foregroundUpload = document.getElementById("foreground-upload");

function saveCanvasAsPNG() {
  const dataURL = canvas.toDataURL("image/png");

  const link = document.createElement("a");
  link.href = dataURL;
  link.download = "canvas_image.png";
  link.click();
}

saveButton.addEventListener("click", saveCanvasAsPNG);
contrastVal.innerHTML = contrastSlider.value / 10.0;
brightnessVal.innerHTML = brightnessSlider.value / 10.0;

function contrastFunction() {
  const val = this.value;
  contrastVal.innerHTML = val;
  contrastSliderProcess(val);
}

function brightnessFunction() {
  const val = this.value;
  brightnessVal.innerHTML = val;
  brightnessSliderProcess(val);
}

brightnessSlider.oninput = brightnessFunction;
contrastSlider.oninput = contrastFunction;

resetButton.onclick = function () {
  contrastVal.innerHTML = 0;
  brightnessVal.innerHTML = 0;
  // handle all options
  postProcessSelection(0);
  filterSelection(0);
  modeSelection(0);
  contrastSliderProcess(0);
  brightnessSliderProcess(0);
};

function uploadFunction(fileUploaded) {
  console.log(fileUploaded);
  if (!fileUploaded) {
    return;
  }
  texture = gl.createTexture();
  const reader = new FileReader();
  reader.onload = function (event) {
    const image = new Image();
    image.src = event.target.result;
    image.onload = function () {
      handleTextureLoaded(texture, image);
    };
  };
  reader.readAsDataURL(fileUploaded);
  return texture;
}

backgroundUpload.onchange = function () {
  const fileUploaded = this.files[0];
  backgroundTexture = uploadFunction(fileUploaded);
};
foregroundUpload.onchange = function () {
  const fileUploaded = this.files[0];
  foregroundTexture = uploadFunction(fileUploaded);
};
