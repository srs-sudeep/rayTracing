---
sidebar_position: 8
---

# Deployment

How to build and deploy RayTracer Studio for production.

## Build for Production

### 1. Build WASM Module

First, compile the C++ code to WebAssembly:

```bash
npm run build:wasm
```

This creates:
- `src/wasm/raytracer.js` - JavaScript loader
- `src/wasm/raytracer.wasm` - WebAssembly binary

### 2. Build React App

```bash
npm run build
```

This creates optimized production files in `dist/`.

## Deployment Options

### Vercel

The recommended deployment platform:

1. **Push to GitHub**
   ```bash
   git add .
   git commit -m "Production build"
   git push
   ```

2. **Connect to Vercel**
   - Go to [vercel.com](https://vercel.com)
   - Import your GitHub repository
   - Vercel auto-detects Vite

3. **Configure** (already in `vercel.json`):
   ```json
   {
     "buildCommand": "npm run build",
     "outputDirectory": "dist",
     "headers": [
       {
         "source": "/(.*)",
         "headers": [
           { "key": "Cross-Origin-Opener-Policy", "value": "same-origin" },
           { "key": "Cross-Origin-Embedder-Policy", "value": "require-corp" }
         ]
       }
     ]
   }
   ```

### GitHub Pages

1. **Install gh-pages**
   ```bash
   npm install --save-dev gh-pages
   ```

2. **Add deploy script** to `package.json`:
   ```json
   {
     "scripts": {
       "deploy": "npm run build && gh-pages -d dist"
     }
   }
   ```

3. **Update `vite.config.js`**:
   ```javascript
   export default defineConfig({
     base: '/rayTracing/',  // Your repo name
     // ...
   });
   ```

4. **Deploy**
   ```bash
   npm run deploy
   ```

### Netlify

1. **Create `netlify.toml`**:
   ```toml
   [build]
     publish = "dist"
     command = "npm run build"

   [[headers]]
     for = "/*"
     [headers.values]
       Cross-Origin-Opener-Policy = "same-origin"
       Cross-Origin-Embedder-Policy = "require-corp"
   ```

2. **Connect and deploy** via Netlify dashboard

### Self-Hosted

1. **Build the app**
   ```bash
   npm run build
   ```

2. **Serve the `dist/` folder** with any static file server:
   ```bash
   npx serve dist
   ```

   Or with nginx:
   ```nginx
   server {
     listen 80;
     root /var/www/raytracer/dist;
     
     add_header Cross-Origin-Opener-Policy "same-origin";
     add_header Cross-Origin-Embedder-Policy "require-corp";
     
     location / {
       try_files $uri $uri/ /index.html;
     }
   }
   ```

## Required Headers

WebAssembly with SharedArrayBuffer requires these headers:

```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

Without these headers, certain WASM features may not work.

## Pre-building WASM

Since Vercel/Netlify don't have Emscripten, you must:

1. Build WASM locally: `npm run build:wasm`
2. Commit the generated files:
   ```bash
   git add src/wasm/raytracer.js src/wasm/raytracer.wasm
   git commit -m "Add compiled WASM"
   ```
3. Push and deploy

:::warning
The `.gitignore` should NOT ignore `src/wasm/` for deployment.
:::

## Documentation Deployment

### Build Documentation

```bash
cd docs
npm install
npm run build
```

### Deploy to GitHub Pages

```bash
cd docs
npm run deploy
```

This publishes to `https://username.github.io/repo/`.

### Deploy to Vercel

Create a separate Vercel project for the `docs/` folder, or use a monorepo setup.

## Environment Variables

No environment variables are required. All configuration is in the code.

## Performance Tips for Production

1. **Use higher resolution** in production (768 or 1024)
2. **Enable compression** on your server (gzip/brotli)
3. **Set cache headers** for `.wasm` files (they don't change often)

```nginx
location ~* \.wasm$ {
  add_header Cache-Control "public, max-age=31536000";
}
```

