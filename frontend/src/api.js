import axios from 'axios';

const BASE_URL = 'http://localhost:9090';

export const processImage = async (payload) => {
  try {
    const response = await axios.post(`${BASE_URL}/`, payload);
    return response.data;
  } catch (error) {
    console.error("Error processing image:", error);
    throw error;
  }
};
