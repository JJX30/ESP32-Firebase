import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyBmQRLzyIPr1VbMwpXkr1iWWSV8q5ahDi0",
  authDomain: "fir-esp32-c1eca.firebaseapp.com",
  databaseURL: "https://fir-esp32-c1eca-default-rtdb.firebaseio.com",
  projectId: "fir-esp32-c1eca",
  storageBucket: "fir-esp32-c1eca.appspot.com",
  messagingSenderId: "18399808270",
  appId: "1:18399808270:web:76d199b45c67e8b4e8e55f",
  databaseURL: "https://fir-esp32-c1eca-default-rtdb.firebaseio.com/",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

export { db };
