#include "fbg.hpp"
#include "TestingTool/fbTestingTool.hpp"

void float_vector_test();
void alpha_composite_test();
void frame_test();
void shape_tests();

using namespace fbg;
using namespace fbtt;

int main() 
{
   float_vector_test();
   alpha_composite_test();
   frame_test();
   shape_tests();

   return 0;
}

void float_vector_test()
{
   MultiTest<V2d<float>> newVector { "Test of V2d vector class" };

   newVector.add_constructor(
      "Default constructor", [](auto & vec) {
         vec = new V2d<float>;
      }
   );

   newVector.add_constructor(
      "Constructor {0, 0}", [](auto & vec) {
         vec = new V2d<float> { 0, 0 };
      }
   );

   newVector.add_test(
      "New vector has size 0", [](auto & vec) {
         assert_equals(vec.size(), 0.0f);
      }
   );

   newVector.add_test(
      "Basic vector arithmetic", [](auto & vec) {
         vec = { 5.0f, -5.0f };
         assert_equals(vec.x,  5.0f, "Setting failed");
         assert_equals(vec.y, -5.0f, "Setting failed");

         vec = V2d<float> { -10.0f, 230.5f } + V2d<float> { 132.5f, -143.0f };
         assert_equals(vec.x,  122.5f, "Addition failed");
         assert_equals(vec.y,   87.5f, "Addition failed");

         vec = V2d<float> { 13.0f, 5.0f } - V2d<float> { 5.0f, 3.0f };
         assert_equals(vec.x, 8.0f, "Subtraction failed");
         assert_equals(vec.y, 2.0f, "Subtraction failed");

         vec = V2d<float> { 64.0f, -32.0f } * 2.5f;
         assert_equals(vec.x, 160.0f, "Multiplication failed");
         assert_equals(vec.y, -80.0f, "Multiplication failed");

         vec = V2d<float> { 160.0f, -80.0f } / 4.0f;
         assert_equals(vec.x,  40.0f, "Division failed");
         assert_equals(vec.y, -20.0f, "Division failed");
      }
   );

   newVector.add_test(
      "Setting arithmetic", [](auto & vec) {
         vec += { 145.0f, -90.0f };
         assert_equals(vec.x, 145.0f, "Addition failed");
         assert_equals(vec.y, -90.0f, "Addition failed");

         vec -= { 55.0f, -135.0f };
         assert_equals(vec.x, 90.0f, "Subtraction failed");
         assert_equals(vec.y, 45.0f, "Subtraction failed");

         vec /= 3.0f;
         assert_equals(vec.x, 30.0f, "Division failed");
         assert_equals(vec.y, 15.0f, "Division failed");

         vec *= 3.0f;
         assert_equals(vec.x, 90.0f, "Multiplication failed");
         assert_equals(vec.y, 45.0f, "Multiplication failed");
      }
   );

   newVector.add_test(
      "Vector conversion from int->float", [](auto & vec) {
         V2d<int> intVec { 13023, -132 };

         vec = intVec;

         assert_equals(vec.x, 13023.0f);
         assert_equals(vec.y, -132.0f);
      }
   );
   
   newVector.add_test(
      "Vector convert from float->int->float", [](auto & vec) {
         V2d<float> vf { -32.0f, 70.0f };
         V2d<int>   vi = vf;
         V2d<float> vitof = vi;

         assert_equals(vf.x, vitof.x);
         assert_equals(vf.y, vitof.y);
      }
   );

   newVector.add_test(
      "Size of vec with x and y is equal to sqrt(x * x + y * y)", [](auto & vec) {
         vec = { 3.0f, 4.0f };
         assert_equals(vec.size(), 5.0f);
      }
   );

   newVector.add_test(
      "size_sq gives squared .size()", [](auto & vec) {
         vec = { -12.0f, 15.0f };
         float sz = vec.size();
         float szsq = vec.size_sq();
         float threshold = 0.0001f;
         assert_true(szsq > sz * sz - threshold);
         assert_true(szsq < sz * sz + threshold);
      }
   );

   newVector.add_test(
      "Check is bound", [](auto & vec) {
         vec = { -100.0f, 20.0f };

         assert_true(vec.is_bound({ -101, 19 }, { -99, 20 }));
         assert_false(vec.is_bound({0, 0}, {-10, -10}));
         assert_false(vec.is_bound({-50.0f, 15.0f}, {-25.0f, 25.0f}));
      }
   );

   newVector.add_test(
      "Normalize -> has size 1", [](auto & vec) {
         vec = { -50.0f, -20.0f };
         vec.normalize();
         assert_equals(vec.size(), 1.0f);
         V2d<float> normalizedVec = vec.normal();
         assert_equals(normalizedVec.size(), 1.0f);
      }
   );

   newVector.add_test(
      "Rotate 45 degrees lends correct angle", [](auto & vec) {
         vec = { 5.0f, 0.0f };
         V2d<float> rotated45 = { 5.0f, 5.0f };

         vec.rotate(quarterPi);
         assert_equals(vec.angle(), rotated45.angle());
      }
   );

   newVector.add_test(
      "Setting size x leads to vector with size x", [](auto & vec) {
         vec = { 42.0f, -23.0f };
         vec.size(500.0f);
         assert_approx(vec.size(), 500.0f);
      }
   );

   newVector.add_test(
      "dot product", [](auto & vec) {
         vec = { 42.0f, 69.0f };
         V2d<float> vec2 { 12.0f, 67.0f };
         assert_equals(dot_prod(vec, vec2), 12.0f * 42.0f + 69.0f * 67.0f);
      }
   );

   newVector.add_test<V2d<float>::NoAngle>(
      "angle of vector with size 0 throws", [](auto & vec) {
         assert_equals(vec.size(), 0);
         vec.angle(20.0f);
      }
   );

   newVector.add_test<fbg::DivisionByZero>(
      "normalizing vector with size 0 throws", [](auto & vec) {
         assert_equals(vec.size(), 0);
         vec.normalize();
      }
   );

   newVector.add_test<fbg::DivisionByZero>(
      "normal of vector with size 0 throws", [](auto & vec) {
         assert_equals(vec.size(), 0);
         V2d<float> n = vec.normal();
      }
   );

   newVector.add_test(
      "limiting vectors size when size is greater than limit", [](auto & vec) {
         vec = { 100.0f, -12.0f };
         vec.limit(50.0f);
         assert_approx(vec.size(), 50.0f);

         vec = { 23.0f, 10.0f };
         float oldsz = vec.size();
         vec.limit(50.0f);
         assert_equals(vec.size(), oldsz);
      }
   );

   newVector.add_test(
      "rotating vector leads to correct angle", [](auto & vec) {
         vec = { 10.0f, 10.0f };
         float oldang = vec.angle();
         vec.rotate(pi);
         assert_approx(vec.angle() - pi, oldang);
      }
   );

   newVector.add_test<V2d<float>::NoAngle>(
      "angle between vectors throws when size == 0", [](auto & vec) {
         V2d<float> a { 1.0f, 1.0f };
         angle_between(a, vec);
      }
   );

   newVector.run();

   std::cout << newVector;

}

void alpha_composite_test()
{
   MultiTest acTest { "Test of alpha compositing algorithms" };

   acTest.add_test(
      "Single color over single color", []() {
         Rgba under { 128, 140, 78, 100 };
         Rgba over  { 40, 130, 240, 150 };

         alpha_composite1(&under, &over);

         assert_equals(under.r, (u_int8_t)  76, "red invalid");
         assert_equals(under.g, (u_int8_t) 134, "green invalid");
         assert_equals(under.b, (u_int8_t) 173, "blue invalid");
         assert_equals(under.a, (u_int8_t) 129, "alpha invalid");
      }
   );

   acTest.add_test(
      "Array of color over array of color (length 8)", []() {
         Rgba under[8];
         Rgba oldUnder[8];
         Rgba over [8];

         srand(clock());
         for (size_t i = 0; i < 8; i++) {

            under[i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };

            oldUnder[i] = under[i];

            over [i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };
         }

         alpha_composite8(under, over);

         for (size_t i = 0; i < 8; i++) {
            assert_equals(under[i].r, ((uint8_t) ((over[i].r * over[i].a + oldUnder[i].r * (255 - over[i].a)) / 256)), "red invalid, index: " + std::to_string(i));
            assert_equals(under[i].g, ((uint8_t) ((over[i].g * over[i].a + oldUnder[i].g * (255 - over[i].a)) / 256)), "green invalid, index: " + std::to_string(i));
            assert_equals(under[i].b, ((uint8_t) ((over[i].b * over[i].a + oldUnder[i].b * (255 - over[i].a)) / 256)), "blue invalid, index: " + std::to_string(i));
            assert_equals(under[i].a, ((uint8_t) ((over[i].a * over[i].a + oldUnder[i].a * (255 - over[i].a)) / 256)), "alpha invalid, index: " + std::to_string(i));
         }
      }
   );

   acTest.add_test(
      "Array of color over array of color (length n)", []() {
         int n = rand() % 20000;

         Rgba * under = new Rgba[n];
         Rgba * oldUnder = new Rgba[n];
         Rgba * over = new Rgba[n];

         for (size_t i = 0; i < n; i++) {

            under[i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };

            oldUnder[i] = under[i];

            over [i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };
         }

         alpha_compositeN(under, over, n);

         for (size_t i = 0; i < n; i++) {
            // within margin 1?
            assert_approx(under[i].r, 
               ((uint8_t) ((over[i].r * over[i].a + oldUnder[i].r * (255 - over[i].a)) / 256)), 
               "red invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].g, 
               ((uint8_t) ((over[i].g * over[i].a + oldUnder[i].g * (255 - over[i].a)) / 256)), 
               "green invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].b, 
               ((uint8_t) ((over[i].b * over[i].a + oldUnder[i].b * (255 - over[i].a)) / 256)), 
               "blue invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].a, 
               ((uint8_t) ((over[i].a * over[i].a + oldUnder[i].a * (255 - over[i].a)) / 256)), 
               "alpha invalid, index: " + std::to_string(i), 1);
         }
      }
   );

   acTest.add_test(
      "One color over array of color (length n)", []() {
         int n = rand() % 20000;

         Rgba * under = new Rgba[n];
         Rgba * oldUnder = new Rgba[n];
         Rgba over;

         for (size_t i = 0; i < n; i++) {

            under[i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };

            oldUnder[i] = under[i];

            over = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                     static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };
         }

         alpha_compositeNC(under, &over, n);

         for (size_t i = 0; i < n; i++) {
            // within margin 1?
            assert_approx(under[i].r, 
               ((uint8_t) ((over.r * over.a + oldUnder[i].r * (255 - over.a)) / 256)), 
               "red invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].g, 
               ((uint8_t) ((over.g * over.a + oldUnder[i].g * (255 - over.a)) / 256)), 
               "green invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].b, 
               ((uint8_t) ((over.b * over.a + oldUnder[i].b * (255 - over.a)) / 256)), 
               "blue invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].a, 
               ((uint8_t) ((over.a * over.a + oldUnder[i].a * (255 - over.a)) / 256)), 
               "alpha invalid, index: " + std::to_string(i), 1);
         }
      }
   );

   acTest.run();

   std::cout << acTest;
}

void frame_test()
{
   MultiTest<Frame> frameTest { "Test of frame class" };
   srand(clock());

   frameTest.add_constructor("Constructor with size n", [](auto * & frame) {
      int x = rand() % 1400 + 10;
      int y = rand() % 1400 + 10;
      frame = new Frame { x, y };
   });

   frameTest.add_constructor("Copy constructor", [](auto * & frame) {
      Frame cp { 200, 300 };
      frame = new Frame { cp };
   });

   frameTest.add_constructor("Move constructor", [](auto * & frame) {
      Frame cp { 200, 300 };
      frame = new Frame { std::move(cp) };
   });

   frameTest.add_test<Frame::OutOfRange>(
      "getting pixel -1, -1 throws out of range", [](auto & frame) {
         frame.get(-1, -1);
      }
   );

   frameTest.add_test<Frame::OutOfRange>(
      "getting pixel (w, h) throws out of range", [](auto & frame) {
         frame.get(frame.w, frame.h);
      }
   );

   frameTest.add_test(
      "New frame is black", [](auto & frame) {
         for (int y = 0; y < frame.h; y++)
            for (int x = 0; x < frame.w; x++) {
               Rgba col = frame.get(x, y); 
               assert_equals(col.r, 0, "Red not 0");
               assert_equals(col.g, 0, "Red not 0");
               assert_equals(col.b, 0, "Red not 0");
               assert_equals(col.a, 0, "Red not 0");
            }
      }
   );

   frameTest.add_test(
      "Setting pixel result in pixel having that color", [](auto & frame) {
         frame.set_pixel(5, 6, { 1, 2, 3, 255 });
         Rgba res { frame.get(5, 6) };
         assert_equals(res.r, 1, "red invalid");
         assert_equals(res.g, 2, "red invalid");
         assert_equals(res.b, 3, "red invalid");
         assert_equals(res.a, 255, "red invalid");
      }
   );

   frameTest.add_test(
      "Test of in_bound", [](auto & frame) {
         assert_true(frame.in_bound(0, 0));
         assert_false(frame.in_bound(frame.w, frame.h));
         assert_false(frame.in_bound(-1, -1));
      }
   );

   frameTest.run();
   std::cout << frameTest;
}

void shape_tests()
{
   MultiTest<Line> lineT { "Test of line" };

   lineT.add_constructor("Default constructor + set to()", [](auto * & line) {
      line = new Line { };
      line->to(20.0f, 10.0f);
   });

   lineT.add_constructor("Constructor with points", [](auto * & line) {
      line = new Line { V2d<float> { 0.0f, 0.0f }, V2d<float> { 20.0f, 10.0f }};
   });

   lineT.add_constructor("Constructor with x, y", [](auto * & line) {
      line = new Line { 0, 0, 20, 10 };
   });

   lineT.add_test(
      "from() sets from() (V2d<float>)", [](auto & line) {
         line.from(V2d<float> {32.0f, 132.0f});
         assert_equals(32.0f, line.from().x);
         assert_equals(132.0f, line.from().y);
      }
   );

   lineT.add_test(
      "from() sets from() (x, y)", [](auto & line) {
         line.from(32.0f, 132.0f);
         assert_equals(32.0f, line.from().x);
         assert_equals(132.0f, line.from().y);
      }
   );

   lineT.add_test(
      "to() sets to() (V2d<float>)", [](auto & line) {
         line.to(V2d<float> {32.0f, 132.0f});
         assert_equals(32.0f, line.to().x);
         assert_equals(132.0f, line.to().y);
      }
   );

   lineT.add_test(
      "to() sets to() (x, y)", [](auto & line) {
         line.to(32.0f, 132.0f);
         assert_equals(32.0f, line.to().x);
         assert_equals(132.0f, line.to().y);
      }
   );

   MultiTest<Rect> rectT { "Test of rectangle" };

   rectT.add_constructor(
      "Corner with point", [](auto * & rect) {
         Rect::MODE = Rect::DrawMode::CORNER;
         rect = new Rect { 0.0f, 0.0f, 100.0f, 200.0f };
      }
   );


   lineT.run();
   std::cout << lineT;
}