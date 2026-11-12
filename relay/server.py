import asyncio
import websockets

# This is the "Middleman" that connects players to the LED Matrix
async def middleman(websocket):
    print("Someone connected to the relay!")
    async for message in websocket:
        # We will add the logic to forward the comma-separated values later
        print(f"Received message: {message}")
        
async def start_server():
    print("Middleman Server starting on port 8765...")
    async with websockets.serve(middleman, "localhost", 8765):
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(start_server())
