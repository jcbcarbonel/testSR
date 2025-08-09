import requests

def get_user_data_v1(user_id):
    """Fetches user data from API v1."""
    base_url = "https://api.example.com/api/v1"
    endpoint = f"{base_url}/users"
    params = {"user_id": user_id}
    try:
        response = requests.get(endpoint, params=params)
        response.raise_for_status()  # Raise an exception for bad status codes
        return response.json()
    except requests.exceptions.RequestException as e:
        print(f"Error fetching data from API v1: {e}")
        return None

# Example usage of the original code
user_data_v1 = get_user_data_v1("12345")
if user_data_v1:
    print("User data from API v1:", user_data_v1)