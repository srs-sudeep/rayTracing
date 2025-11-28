import './InfoModal.css';

function InfoModal({ onClose }) {
  return (
    <div className="modal-overlay" onClick={onClose}>
      <div className="modal-content" onClick={(e) => e.stopPropagation()}>
        <button className="modal-close" onClick={onClose}>×</button>
        
        <div className="modal-header">
          <h2>About RayTracer Studio</h2>
        </div>

        <div className="modal-body">
          <section className="info-section">
            <h3>🎯 What is this?</h3>
            <p>
              RayTracer Studio is a <strong>real-time CPU ray tracer</strong> that runs 
              entirely in your browser. The rendering engine is written in C++ and compiled 
              to WebAssembly, allowing near-native performance with the convenience of a web app.
            </p>
          </section>

          <section className="info-section">
            <h3>⚡ Technology Stack</h3>
            <div className="tech-grid">
              <div className="tech-item">
                <span className="tech-icon">🔧</span>
                <div>
                  <strong>C++</strong>
                  <span>Ray tracing engine</span>
                </div>
              </div>
              <div className="tech-item">
                <span className="tech-icon">🌐</span>
                <div>
                  <strong>WebAssembly</strong>
                  <span>Near-native speed</span>
                </div>
              </div>
              <div className="tech-item">
                <span className="tech-icon">⚛️</span>
                <div>
                  <strong>React</strong>
                  <span>Interactive UI</span>
                </div>
              </div>
              <div className="tech-item">
                <span className="tech-icon">📦</span>
                <div>
                  <strong>Emscripten</strong>
                  <span>C++ to WASM compiler</span>
                </div>
              </div>
            </div>
          </section>

          <section className="info-section">
            <h3>✨ Features</h3>
            <ul className="feature-list">
              <li>🔴 Multiple scene presets with different sphere arrangements</li>
              <li>🪞 Recursive reflections with Fresnel effect</li>
              <li>💡 Blinn-Phong specular highlights</li>
              <li>🌑 Hard shadows with shadow rays</li>
              <li>📷 Interactive camera orbit and zoom</li>
              <li>🎨 Customizable materials and colors</li>
              <li>📐 3D ground plane with Blender-style grid</li>
            </ul>
          </section>

          <section className="info-section">
            <h3>🧮 How Ray Tracing Works</h3>
            <p>
              For each pixel, a ray is cast from the camera into the scene. When it hits 
              an object, we calculate lighting using the Blinn-Phong model. If the surface 
              is reflective, we bounce the ray and repeat—up to the max bounce limit.
            </p>
          </section>

          <section className="info-section">
            <h3>🚀 Performance Tips</h3>
            <ul className="feature-list">
              <li>Lower resolution (256-512) for smooth interaction</li>
              <li>Reduce max bounces for faster renders</li>
              <li>Fewer spheres = faster render times</li>
            </ul>
          </section>
        </div>

        <div className="modal-footer">
          <p>
            Made with ❤️ by{' '}
            <a 
              href="https://github.com/srs-sudeep" 
              target="_blank" 
              rel="noopener noreferrer"
              className="author-link"
            >
              SRS
            </a>
          </p>
        </div>
      </div>
    </div>
  );
}

export default InfoModal;

