import pygame
import sys

# Constants
SCALE = 3
BLOCK_SIZE = 32

SCREEN_WIDTH = 400 * SCALE
SCREEN_HEIGHT = 300 * SCALE

# a player is two blocks tall
PLAYER_SIZE = (BLOCK_SIZE * 2) * SCALE
background_image = pygame.image.load('res/textures/grid.png')
background_rect = background_image.get_rect()
print(background_image,background_rect)

# Initialize Pygame
pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('Game')
clock = pygame.time.Clock()

# Load images
player_image = pygame.image.load('res/textures/player.png')
player_rect = player_image.get_rect()

# Set initial player position
player_rect.centerx = SCREEN_WIDTH // 2
player_rect.centery = SCREEN_HEIGHT // 2

# Game loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # Move player
    keys = pygame.key.get_pressed()
    if keys[pygame.K_UP]:
        player_rect.centery -= 5
    if keys[pygame.K_DOWN]:
        player_rect.centery += 5
    if keys[pygame.K_LEFT]:
        player_rect.centerx -= 5
    if keys[pygame.K_RIGHT]:
        player_rect.centerx += 5

    # Update screen
    screen.fill((255, 255, 255))
    screen.blit(background_image, background_rect)

    # Check if player is at the edge of the screen
    if player_rect.left < 0:
        background_rect.left += 5
    elif player_rect.right > SCREEN_WIDTH:
        background_rect.right -= 5
    if player_rect.top < 0:
        background_rect.top += 5
    elif player_rect.bottom > SCREEN_HEIGHT:
        background_rect.bottom -= 5

    # Draw player
    screen.blit(player_image, player_rect)

    pygame.display.flip()
    clock.tick(60)

