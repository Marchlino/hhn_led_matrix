import asyncio
import websockets

# Keep track of the connections
matrix_connection = None
players = {}
next_player_id = 1

async def handle_matrix(websocket):
    """Handles the single connection from the LED Matrix."""
    global matrix_connection
    matrix_connection = websocket
    print("LED Matrix connected to the Middleman!")
    
    try:
        async for message in websocket:
            # Matrix sends messages like "1,VIBRATE" (PlayerID, Command)
            if "," in message:
                target_id, payload = message.split(",", 1)
                
                if target_id.upper() == "ALL":
                    # Broadcast to everyone
                    for p_ws in players.values():
                        await p_ws.send(payload)
                elif target_id.isdigit() and int(target_id) in players:
                    # Send to a specific player
                    await players[int(target_id)].send(payload)
    finally:
        matrix_connection = None
        print("LED Matrix disconnected.")

async def handle_player(websocket):
    """Handles connections from individual players' phones."""
    global next_player_id
    my_id = next_player_id
    next_player_id += 1
    
    players[my_id] = websocket
    print(f"Player {my_id} connected!")

    try:
        async for message in websocket:
            # When player 1 sends "MOVE,UP", forward it to the Matrix as "1,MOVE,UP"
            if matrix_connection:
                await matrix_connection.send(f"{my_id},{message}")
    finally:
        # Clean up when they leave
        players.pop(my_id, None)
        if matrix_connection:
            await matrix_connection.send(f"{my_id},DISCONNECTED")
        print(f"Player {my_id} disconnected.")

async def middleman(websocket):
    """Routes the connection to the right place."""
    # The Matrix connects to ws://localhost:8765/matrix
    # Players connect to ws://localhost:8765/
    path = getattr(websocket, "request", None)
    route = path.path if path else getattr(websocket, "path", "/")
    
    if route == "/matrix":
        await handle_matrix(websocket)
    else:
        await handle_player(websocket)

async def start_server():
    print("Middleman Server starting on port 8765...")
    async with websockets.serve(middleman, "0.0.0.0", 8765):
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(start_server())
