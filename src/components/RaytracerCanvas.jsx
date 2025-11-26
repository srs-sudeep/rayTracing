import { useRef, useEffect, useCallback } from 'react';

const CANVAS_WIDTH = 512;
const CANVAS_HEIGHT = 512;

function RaytracerCanvas({ wasmModule, lightPosition, onRenderTime }) {
  const canvasRef = useRef(null);
  const animationFrameRef = useRef(null);

  const renderFrame = useCallback(() => {
    if (!wasmModule || !canvasRef.current) return;

    const canvas = canvasRef.current;
    const ctx = canvas.getContext('2d');

    // Update light position in C++
    wasmModule.updateLight(lightPosition.x, lightPosition.y, lightPosition.z);

    // Time the render
    const startTime = performance.now();

    // Call C++ render function - returns a std::vector<uint8_t>
    const pixelVector = wasmModule.render(CANVAS_WIDTH, CANVAS_HEIGHT);

    const endTime = performance.now();
    onRenderTime(endTime - startTime);

    // Get the size and create a view into the vector data
    const size = pixelVector.size();
    const pixelData = new Uint8ClampedArray(size);

    // Copy data from the C++ vector
    for (let i = 0; i < size; i++) {
      pixelData[i] = pixelVector.get(i);
    }

    // IMPORTANT: Delete the C++ vector to prevent memory leaks
    pixelVector.delete();

    // Create ImageData and draw to canvas
    const imageData = new ImageData(pixelData, CANVAS_WIDTH, CANVAS_HEIGHT);
    ctx.putImageData(imageData, 0, 0);
  }, [wasmModule, lightPosition, onRenderTime]);

  useEffect(() => {
    // Render when light position changes
    renderFrame();
  }, [renderFrame]);

  return (
    <div className="canvas-wrapper">
      <canvas
        ref={canvasRef}
        width={CANVAS_WIDTH}
        height={CANVAS_HEIGHT}
        className="raytracer-canvas"
      />
      <div className="canvas-info">
        <span>{CANVAS_WIDTH} × {CANVAS_HEIGHT}</span>
      </div>
    </div>
  );
}

export default RaytracerCanvas;

