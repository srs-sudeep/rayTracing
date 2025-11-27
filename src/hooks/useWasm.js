import { useState, useEffect } from 'react';

/**
 * Custom hook for loading and managing the WebAssembly module
 */
export function useWasm() {
  const [wasmModule, setWasmModule] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    async function loadWasm() {
      try {
        const createModule = (await import('../wasm/raytracer.js')).default;
        const module = await createModule();
        setWasmModule(module);
        setLoading(false);
      } catch (err) {
        console.error('Failed to load WASM module:', err);
        setError(err.message);
        setLoading(false);
      }
    }

    loadWasm();
  }, []);

  return { wasmModule, loading, error };
}

export default useWasm;
