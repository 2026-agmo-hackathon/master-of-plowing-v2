import { defineConfig } from 'vitest/config'
import react from '@vitejs/plugin-react'

export default defineConfig({
  plugins: [react()],
  // The CustomUI is served from a feature-relative path on the device, so every
  // asset URL must stay relative.
  base: './',
  server: { port: 3002, cors: true },
  build: { target: 'esnext' },
})
