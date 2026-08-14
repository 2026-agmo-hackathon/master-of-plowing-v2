import { defineConfig, devices } from '@playwright/test';

export default defineConfig({
  testDir: './tests/visual',
  reporter: 'list',
  expect: {
    timeout: 30_000,
  },
  use: {
    baseURL: 'http://localhost:3002',
    viewport: { width: 1280, height: 720 },
  },
  webServer: {
    command: 'npm run dev',
    url: 'http://localhost:3002',
    reuseExistingServer: !process.env.CI,
    timeout: 60_000,
  },
  projects: [
    {
      name: 'chromium',
      use: { ...devices['Desktop Chrome'] },
    },
  ],
});
