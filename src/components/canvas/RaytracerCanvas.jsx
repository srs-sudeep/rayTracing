import { useRef, useEffect, useCallback, useState } from 'react';
import './RaytracerCanvas.css';

function RaytracerCanvas({ 
  wasmModule, 
  lights, 
  material, 
  camera,
  view,
  scenePreset,
  onCameraChange,
  onRenderTime 
}) {
  const canvasRef = useRef(null);
  const containerRef = useRef(null);
  const [isDragging, setIsDragging] = useState(false);
  const [displaySize, setDisplaySize] = useState({ width: 700, height: 700 });
  const [isMobile, setIsMobile] = useState(window.innerWidth <= 480);
  const lastMousePos = useRef({ x: 0, y: 0 });
  const renderRequestRef = useRef(null);
  const lastPresetRef = useRef(scenePreset);

  // Calculate display size to fill container while maintaining square aspect
  useEffect(() => {
    const updateSize = () => {
      if (containerRef.current) {
        const rect = containerRef.current.getBoundingClientRect();
        const mobile = window.innerWidth <= 480;
        setIsMobile(mobile);
        const padding = mobile ? 10 : 20;
        const maxSize = Math.min(rect.width - padding, rect.height - padding);
        const minSize = window.innerWidth <= 360 ? 200 : 250;
        const size = Math.max(minSize, maxSize);
        setDisplaySize({ width: size, height: size });
      }
    };

    updateSize();
    window.addEventListener('resize', updateSize);
    // Also listen for orientation changes on mobile
    window.addEventListener('orientationchange', () => setTimeout(updateSize, 100));
    const timeout = setTimeout(updateSize, 100);
    
    return () => {
      window.removeEventListener('resize', updateSize);
      window.removeEventListener('orientationchange', updateSize);
      clearTimeout(timeout);
    };
  }, []);

  // Optimized render function
  const renderFrame = useCallback(() => {
    if (!wasmModule || !canvasRef.current) return;

    const canvas = canvasRef.current;
    const ctx = canvas.getContext('2d');
    const resolution = view.resolution;

    // Update scene preset if changed
    if (lastPresetRef.current !== scenePreset) {
      wasmModule.loadScenePreset(scenePreset);
      lastPresetRef.current = scenePreset;
    }

    // Update all lights
    lights.forEach((light, index) => {
      wasmModule.setLightPosition(index, light.x, light.y, light.z);
      wasmModule.setLightColor(index, light.color.r, light.color.g, light.color.b);
      wasmModule.setLightIntensity(index, light.intensity);
    });

    // Material updates
    wasmModule.updateMaterial(material.specular, material.shininess, material.reflectivity);
    wasmModule.updateSphereColor(material.color.r, material.color.g, material.color.b);
    wasmModule.updateMaterialTransparency(material.transparency, material.refractiveIndex);
    
    // Camera updates
    wasmModule.updateCamera(camera.position.x, camera.position.y, camera.position.z);
    wasmModule.setCameraTarget(camera.target.x, camera.target.y, camera.target.z);
    wasmModule.setCameraFov(camera.fov);
    
    // View settings
    wasmModule.setShowGroundPlane(view.showGroundPlane);
    wasmModule.setShowGrid(view.showGrid);
    wasmModule.setGridScale(view.gridScale);
    wasmModule.updateGroundReflectivity(view.groundReflectivity);
    wasmModule.setMaxReflectionDepth(view.maxBounces);
    wasmModule.setAntiAliasing(view.antiAliasing);
    
    // Soft shadow settings
    wasmModule.setSoftShadows(view.softShadows);
    wasmModule.setShadowSamples(view.shadowSamples);
    // Apply light radius to all lights
    for (let i = 0; i < lights.length; i++) {
      wasmModule.setLightRadius(i, view.lightRadius);
    }

    // Time the render
    const startTime = performance.now();
    const pixelVector = wasmModule.render(resolution, resolution);
    const endTime = performance.now();
    onRenderTime(endTime - startTime);

    // Copy data from C++ vector
    const size = pixelVector.size();
    const pixelData = new Uint8ClampedArray(size);
    for (let i = 0; i < size; i++) {
      pixelData[i] = pixelVector.get(i);
    }
    pixelVector.delete();

    // Draw to canvas
    const imageData = new ImageData(pixelData, resolution, resolution);
    
    if (canvas.width !== resolution) {
      canvas.width = resolution;
      canvas.height = resolution;
    }
    
    ctx.putImageData(imageData, 0, 0);
  }, [wasmModule, lights, material, camera, view, scenePreset, onRenderTime]);

  // Debounced render
  useEffect(() => {
    if (renderRequestRef.current) {
      cancelAnimationFrame(renderRequestRef.current);
    }
    renderRequestRef.current = requestAnimationFrame(renderFrame);
    
    return () => {
      if (renderRequestRef.current) {
        cancelAnimationFrame(renderRequestRef.current);
      }
    };
  }, [renderFrame]);

  // Mouse handlers for camera orbit
  const handleMouseDown = (e) => {
    setIsDragging(true);
    lastMousePos.current = { x: e.clientX, y: e.clientY };
  };

  const handleMouseMove = (e) => {
    if (!isDragging || !wasmModule) return;

    const deltaX = e.clientX - lastMousePos.current.x;
    const deltaY = e.clientY - lastMousePos.current.y;
    lastMousePos.current = { x: e.clientX, y: e.clientY };

    wasmModule.orbitCamera(deltaX, deltaY);
    
    // Sync camera position back to React state
    onCameraChange({
      ...camera,
      position: {
        x: wasmModule.getCameraX(),
        y: wasmModule.getCameraY(),
        z: wasmModule.getCameraZ()
      }
    });
  };

  const handleMouseUp = () => {
    setIsDragging(false);
  };

  const handleWheel = (e) => {
    if (!wasmModule) return;
    e.preventDefault();
    
    const delta = e.deltaY * 0.01;
    wasmModule.zoomCamera(delta);
    
    onCameraChange({
      ...camera,
      position: {
        x: wasmModule.getCameraX(),
        y: wasmModule.getCameraY(),
        z: wasmModule.getCameraZ()
      }
    });
  };

  // Touch handlers for mobile
  const handleTouchStart = (e) => {
    if (e.touches.length === 1) {
      setIsDragging(true);
      lastMousePos.current = { x: e.touches[0].clientX, y: e.touches[0].clientY };
    }
  };

  const handleTouchMove = (e) => {
    if (!isDragging || !wasmModule || e.touches.length !== 1) return;
    e.preventDefault();

    const deltaX = e.touches[0].clientX - lastMousePos.current.x;
    const deltaY = e.touches[0].clientY - lastMousePos.current.y;
    lastMousePos.current = { x: e.touches[0].clientX, y: e.touches[0].clientY };

    wasmModule.orbitCamera(deltaX, deltaY);
    
    onCameraChange({
      ...camera,
      position: {
        x: wasmModule.getCameraX(),
        y: wasmModule.getCameraY(),
        z: wasmModule.getCameraZ()
      }
    });
  };

  const handleTouchEnd = () => {
    setIsDragging(false);
  };

  return (
    <div className="canvas-container" ref={containerRef}>
      <canvas
        ref={canvasRef}
        width={view.resolution}
        height={view.resolution}
        className={`raytracer-canvas ${isDragging ? 'dragging' : ''}`}
        style={{ 
          width: displaySize.width, 
          height: displaySize.height 
        }}
        onMouseDown={handleMouseDown}
        onMouseMove={handleMouseMove}
        onMouseUp={handleMouseUp}
        onMouseLeave={handleMouseUp}
        onWheel={handleWheel}
        onTouchStart={handleTouchStart}
        onTouchMove={handleTouchMove}
        onTouchEnd={handleTouchEnd}
        onTouchCancel={handleTouchEnd}
      />
      <div className="canvas-badge top-left">
        {view.resolution}² • {lights.length}💡{view.antiAliasing > 0 && ` • AA`}{view.softShadows && ` • Soft`}
      </div>
      <div className="canvas-badge bottom-right">
        {isMobile ? 'Touch to orbit' : 'Drag to orbit • Scroll to zoom'}
      </div>
    </div>
  );
}

export default RaytracerCanvas;
