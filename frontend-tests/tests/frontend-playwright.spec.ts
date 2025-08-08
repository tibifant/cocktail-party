import { test, expect } from '@playwright/test';

const BASE_URL = 'http://localhost:61919';

test.describe('Cocktail Party App', () => {
  test.beforeEach(async ({ page }) => {
    await page.goto(`file://${process.cwd()}/frontend/index.html`);
    //await page.goto(BASE_URL);
  });

  test('get list', async ({ page }) => {
    const list = page.locator('#list li');
    await expect(list.first()).toBeVisible();
  });

  test('get cocktail details', async ({ page }) => {
    const firstCocktail = page.locator('#list li').first();
    await firstCocktail.click();

    const title = page.locator('#title');
    const author = page.locator('#author');
    const instructions = page.locator('#instructions');

    // Check that title, author, and instructions are populated
    await expect(title).not.toHaveText('');
    await expect(author).toContainText('by ');
    await expect(instructions).not.toHaveText('');
  });

  test('add cocktail', async ({ page }) => {
    const addButton = page.locator('#add');
    await addButton.click();

    // Verify that a cocktail is added and its title appears
    const title = page.locator('#title');
    await expect(title).not.toHaveText(''); // title should be populated

    // Check if the list contains the new cocktail title
    const newTitle = await title.textContent();
    const listItem = page.locator(`#list li:has-text("${newTitle}")`);
    await expect(listItem).toBeVisible();
  });

  test('update cocktail', async ({ page }) => {
    const firstCocktail = page.locator('#list li').first();
    await firstCocktail.click();
    
    // Verify instructions change (update button generates new instructions)
    const instructions = page.locator('#instructions');
    const oldInstructions = await instructions.textContent();
    
    const updateButton = page.locator('#update');
    await updateButton.click();
    await expect(instructions).not.toHaveText(oldInstructions || '');
  });

  test('remove cocktail', async ({ page }) => {
    const firstCocktail = page.locator('#list li').first();
    await firstCocktail.click();
  
    // Wait for title to be populated before reading it
    const titleLocator = page.locator('#title');
    await expect(titleLocator).not.toHaveText('');
    const title = await titleLocator.textContent();
  
    const removeButton = page.locator('#remove');
    await removeButton.click();
  
    // Wait for the list to update after removal
    await expect(page.locator(`#list li:has-text("${title}")`)).toHaveCount(0);
  });
});
