# Compose 
## Program sederhana untuk menggabungkan beberapa file CSS menjadi satu.
         

## A) Cara penggunaan

   Katakanlah terdapat program `compose.exe`, file `script.cpss`, serta subfolder `components/`.
   Lalu di dalam folder `components/` terdapat file `base.css` serta `navbar.css`
   
   
   Dengan isi dari file `script.cpss` adalah sebagai berikut:
      
      {% compose 'style.css' %}
      
      {% insertfile 'components/base.css' %}
      {% insertfile 'components/navbar.css' %}
   
   
   Kemudian dijalankan perintah di CMD sebagai berikut:
      
      compose script.cpss
   
   
   Maka program akan membaca file `script.cpss` dan akan mengeksekusi
   perintah sesuai dengan tag-tag yang ada di dalamnya.
   
   Dalam contoh ini, program akan menggabungkan file `components/base.css` 
   dan `components/navbar.css` menjadi satu. 
   
   File output akan bernama `style.css` yang akan terletak di folder yang 
   sama dengan tempat `compose.exe` berada
   
   
## B) Tujuan
   Untuk latihan saja
   
   
   
   
   
   
   
   
   
   
