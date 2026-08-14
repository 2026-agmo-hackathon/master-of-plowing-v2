import { defineConfig } from 'vitest/config'
import react from '@vitejs/plugin-react'
import simulatorApiPlugin from './vite-api-plugin'
export default defineConfig({
  plugins: [react(), simulatorApiPlugin()],
  base: './',
  server: { port: 3002, cors: true },
  build: { target: 'esnext' },
  test: { exclude: ['**/node_modules/**', '**/dist/**', 'tests/visual/**'] },
})
