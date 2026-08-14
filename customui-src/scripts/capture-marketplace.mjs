// Capture script: takes a 1024x1024 screenshot of the app's initial entry screen
// Usage: node scripts/capture-marketplace.mjs (from ui/ directory)
import { chromium } from 'playwright';
import { fileURLToPath } from 'url';
import path from 'path';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const outputPath = path.resolve(__dirname, '../marketplace-assets/screenshot.png');

(async () => {
  const browser = await chromium.launch({ headless: true });
  const context = await browser.newContext({
    viewport: { width: 1024, height: 1024 },
  });
  const page = await context.newPage();

  await page.goto('http://localhost:3002', { waitUntil: 'networkidle', timeout: 60000 });

  // Hide the "Connection lost. Reconnecting..." banner — dev server has no backend so it always shows.
  // Target leaf-level role="status" elements to avoid hiding ancestor containers that also contain the text.
  await page.evaluate(() => {
    document.querySelectorAll('div[role="status"][aria-live="polite"]').forEach(d => {
      if (d.textContent?.trim().startsWith('Connection lost')) {
        d.style.display = 'none';
      }
    });
  });

  // Wait additional 2s for 3D scene to fully render
  await page.waitForTimeout(2000);

  await page.screenshot({
    path: outputPath,
    fullPage: false,
  });

  console.log(`Screenshot saved to: ${outputPath}`);
  await browser.close();
})();
